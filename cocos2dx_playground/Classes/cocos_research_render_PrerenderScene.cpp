#include "cocos_research_render_PrerenderScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCRenderTexture.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccShaders.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"

#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace
{
	const char* CustomeShaderPath = "shaders/shader_DarkScale.fsh";
}

namespace cocos_research_render
{
	PrerenderScene::PrerenderScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mActorNode( nullptr )
		, mTileMapNode( nullptr )
		, mRenderTextureNode( nullptr )

		, mbInputBlock( false )
	{}

	Scene* PrerenderScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) PrerenderScene( back_to_the_previous_scene_callback );
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool PrerenderScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();
		const Vec2 visibleCenter(
			visibleOrigin.x + ( visibleSize.width * 0.5f )
			, visibleOrigin.y + ( visibleSize.height * 0.5f )
		);

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Root";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[SPACE] : Do";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 8 );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}
			
		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Research
		//
		{
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B::BLACK, visibleSize.width * 0.5f, visibleSize.height );
				background_layer->setPositionX( visibleCenter.x );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Render Node
			//
			{
				const step_defender::game::TileMapNode::Config stage_config{ 6u, 6u };

				// Load Tile Config
				cpg::TileSheetConfiguration tile_sheet_configuration;
				CCASSERT( tile_sheet_configuration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

				auto root_node = Node::create();
				root_node->setContentSize( Size(
					tile_sheet_configuration.GetTileWidth() * stage_config.MapWidth
					, tile_sheet_configuration.GetTileHeight() * stage_config.MapHeight
				) );
				root_node->setPosition( Vec2(
					( visibleSize.width * 0.25f ) - ( root_node->getContentSize().width * 0.5f )
					, visibleCenter.y - root_node->getContentSize().height * 0.5f
				) );
				addChild( root_node );

				{
					mTileMapNode = step_defender::game::TileMapNode::create(
						stage_config
						, tile_sheet_configuration
					);
					mTileMapNode->FillAll( 4, 0 );
					root_node->addChild( mTileMapNode );
				}

				{
					auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_dummy_01.png" );
					sprite->getTexture()->setAntiAliasTexParameters();
					sprite->setPosition( tile_sheet_configuration.GetTileWidth() * 3u, tile_sheet_configuration.GetTileHeight() * 3u );
					root_node->addChild( sprite, 1 );

					mActorNode = sprite;
				}
			}

			//
			//
			//
			{
				mRenderTextureNode = RenderTexture::create( visibleSize.width * 0.5f, visibleSize.height );
				mRenderTextureNode->setContentSize( Size( visibleSize.width * 0.5f, visibleSize.height ) );
				mRenderTextureNode->setVisible( false );
				mRenderTextureNode->setAutoDraw( false );
				mRenderTextureNode->setClearFlags( GL_COLOR_BUFFER_BIT );
				mRenderTextureNode->getSprite()->getTexture()->setAliasTexParameters();
				addChild( mRenderTextureNode );

				auto sprite = Sprite::createWithTexture( mRenderTextureNode->getSprite()->getTexture() );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setScaleY( -1 );
				sprite->setPosition( visibleCenter.x, visibleOrigin.y + visibleSize.height );
				addChild( sprite );
				{
					// Load
					const auto shader_source = FileUtils::getInstance()->getStringFromFile( FileUtils::getInstance()->fullPathForFilename( CustomeShaderPath ) );
					auto gl_program = GLProgram::createWithByteArrays( ccPositionTextureColor_noMVP_vert, shader_source.c_str() );
					auto gl_program_state = GLProgramState::getOrCreateWithGLProgram( gl_program );

					sprite->setGLProgramState( gl_program_state );
				}
			}
		}

		return true;
	}

	void PrerenderScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PrerenderScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void PrerenderScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void PrerenderScene::test_UpdateEnd( float )
	{
		mTileMapNode->setPositionX( mTileMapNode->getPositionX() + 30.f );


		//
		//
		//
		{
			mRenderTextureNode->beginWithClear( mRenderTextureNode->getClearColor().r, mRenderTextureNode->getClearColor().g, mRenderTextureNode->getClearColor().b, mRenderTextureNode->getClearColor().a );
			mTileMapNode->visit(
				_director->getRenderer()
				, _director->getMatrix( cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW )
				, true
			);
			mRenderTextureNode->end();
		}


		unscheduleAllCallbacks();
		mbInputBlock = false;
	}

	void PrerenderScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_SPACE:
			scheduleOnce( schedule_selector( PrerenderScene::test_UpdateEnd ), 0.f );
			mbInputBlock = true;
			return;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
