#include "cocos_research_render_Prerender2Scene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCRenderTexture.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCRenderer.h"

#include "cpg_node_PivotNode.h"
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
	Prerender2Scene::Prerender2Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mActorNode( nullptr )
		, mTileMapNode( nullptr )
		, mRenderTextureNode( nullptr )

		, mbInputBlock( false )
		, mCapturePivot()
	{}

	Scene* Prerender2Scene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) Prerender2Scene( back_to_the_previous_scene_callback );
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

	bool Prerender2Scene::init()
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
			ss << "[SPACE] : Capture";
			ss << cpg::linefeed;
			ss << "[Arrow] : Move Actor";

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
			// Left
			{
				auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ), visibleSize.width * 0.5f, visibleSize.height );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			// Right
			{
				auto background_layer = LayerColor::create( Color4B::BLACK, visibleSize.width * 0.5f, visibleSize.height );
				background_layer->setPositionX( visibleCenter.x );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}
		}

		//
		// Research
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
				, visibleCenter.y - ( root_node->getContentSize().height * 0.5f )
			) );
			addChild( root_node );

			//
			// Pivot
			//
			{
				root_node->addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
			}

			//
			// Tile Map
			//
			{
				mTileMapNode = step_defender::game::TileMapNode::create(
					stage_config
					, tile_sheet_configuration
				);
				mTileMapNode->FillAll( 4, 0 );
				root_node->addChild( mTileMapNode );

				mTileMapNode->UpdateTile( 0, 0, 2, 0 );
				mTileMapNode->UpdateTile( stage_config.MapWidth - 1, stage_config.MapHeight - 1, 1, 0 );
			}

			//
			// Actor
			//
			{
				auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_dummy_01.png" );
				sprite->getTexture()->setAntiAliasTexParameters();
				sprite->setPosition( tile_sheet_configuration.GetTileWidth() * 3u, tile_sheet_configuration.GetTileHeight() * 3u );
				root_node->addChild( sprite, 1 );

				mActorNode = sprite;
			}

			//
			// Util 4 Capture
			//
			{
				mRenderTextureNode = RenderTexture::create( root_node->getContentSize().width, root_node->getContentSize().height );
				mRenderTextureNode->setVisible( false );
				mRenderTextureNode->setAutoDraw( false );
				mRenderTextureNode->setClearFlags( GL_COLOR_BUFFER_BIT );
				mRenderTextureNode->setClearColor( Color4F::ORANGE );
				mRenderTextureNode->getSprite()->getTexture()->setAliasTexParameters();
				addChild( mRenderTextureNode );

			}

			//
			// Show Captured Image
			//
			{
				auto sprite = Sprite::createWithTexture( mRenderTextureNode->getSprite()->getTexture() );
				sprite->setPosition( Vec2( visibleSize.width * 0.75f, visibleCenter.y ) );
				sprite->setFlippedY( true );
				addChild( sprite );
			}

			//
			// Capture Arrea
			//
			{
				auto draw_node = DrawNode::create();
				draw_node->drawRect( Vec2::ZERO, Vec2( root_node->getContentSize().width, root_node->getContentSize().height ), Color4F::GREEN );
				draw_node->setPosition( root_node->getPosition() );
				addChild( draw_node, 100 );

				mCapturePivot = draw_node->getPosition();
			}
		}

		return true;
	}

	void Prerender2Scene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( Prerender2Scene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void Prerender2Scene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void Prerender2Scene::test_UpdateEnd( float )
	{
		//
		//
		//
		{
			const auto last_position = mTileMapNode->getPosition();
			auto temp_position = mTileMapNode->getParent()->convertToWorldSpace( mTileMapNode->getPosition() );
			temp_position -= mCapturePivot;

			//
			// Move 2 Capture Position
			//
			mTileMapNode->setPosition( temp_position );

			//
			// Capture
			//
			mRenderTextureNode->beginWithClear( mRenderTextureNode->getClearColor().r, mRenderTextureNode->getClearColor().g, mRenderTextureNode->getClearColor().b, mRenderTextureNode->getClearColor().a );
			mTileMapNode->visit(
				_director->getRenderer()
				, _director->getMatrix( cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW )
				, true
			);
			mRenderTextureNode->end();

			//
			// Forced Render
			//
			_director->getRenderer()->render(); // Important~!~!~!


			//
			// Rollback
			//
			mTileMapNode->setPosition( last_position );
			mTileMapNode->setScaleX( -mTileMapNode->getScaleX() ); // - for "dirty"
			mTileMapNode->setScaleX( -mTileMapNode->getScaleX() ); // - for "dirty"
		}


		unscheduleAllCallbacks();
		mbInputBlock = false;
	}

	void Prerender2Scene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_SPACE:
			scheduleOnce( schedule_selector( Prerender2Scene::test_UpdateEnd ), 0.f );
			mbInputBlock = true;
			return;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			mActorNode->setPositionX( mActorNode->getPositionX() + 10.f );
			return;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			mActorNode->setPositionX( mActorNode->getPositionX() - 10.f );
			return;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			mActorNode->setPositionY( mActorNode->getPositionY() + 10.f );
			return;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			mActorNode->setPositionY( mActorNode->getPositionY() - 10.f );
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
