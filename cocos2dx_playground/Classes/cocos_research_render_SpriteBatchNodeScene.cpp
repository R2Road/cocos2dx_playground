#include "cocos_research_render_SpriteBatchNodeScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCRenderer.h"

#include "platform/CCFileUtils.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccShaders.h"

#include "cpg_Clamp.h"
#include "cpg_node_PivotNode.h"
#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"

#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace
{
	const char* CustomeShaderPath = "shaders/cocos_research_render_SpriteBatchNodeAlphaScene.fsh";
}

namespace cocos_research_render
{
	SpriteBatchNodeScene::SpriteBatchNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mTileMapNode( nullptr )
		, mTileMapOpacityLabel( nullptr )
	{}

	Scene* SpriteBatchNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) SpriteBatchNodeScene( back_to_the_previous_scene_callback );
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

	bool SpriteBatchNodeScene::init()
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
			ss << "[W/S] : Alpha Up/Down";

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
			auto background_layer = LayerColor::create( Color4B( 37, 18, 53, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// 
		//
		{
			mTileMapOpacityLabel = Label::createWithTTF( "---", cpg::StringTable::GetFontPath(), 8 );
			mTileMapOpacityLabel->setAnchorPoint( Vec2( 1.f, 1.f ) );
			mTileMapOpacityLabel->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width, visibleSize.height )
			);
			addChild( mTileMapOpacityLabel, std::numeric_limits<int>::max() );
		}

		//
		// Practice : Load and Caching
		//
		{
			// Load
			const auto shader_source = FileUtils::getInstance()->getStringFromFile( FileUtils::getInstance()->fullPathForFilename( CustomeShaderPath ) );
			auto gl_program = GLProgram::createWithByteArrays( ccPositionTextureColor_vert, shader_source.c_str() );

			// Caching
			GLProgramCache::getInstance()->addGLProgram( gl_program, "cocos_research_render_SpriteBatchNodeAlphaScene" );
		}

		//
		// Research
		//
		{
			const step_defender::game::TileMapNode::Config stage_config{ 6u, 6u };

			// Load Tile Config
			cpg::TileSheetConfiguration tile_sheet_configuration;
			CCASSERT( tile_sheet_configuration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

			//
			// Tile Map
			//
			{
				mTileMapNode = step_defender::game::TileMapNode::create(
					stage_config
					, tile_sheet_configuration
				);
				mTileMapNode->setPosition( Vec2(
					visibleCenter.x - ( mTileMapNode->getContentSize().width * 0.5f )
					, visibleCenter.y - ( mTileMapNode->getContentSize().height * 0.5f )
				) );
				mTileMapNode->FillAll( 4, 0 );
				addChild( mTileMapNode );
				{
					//
					// Get Cached Program
					//
					auto gl_program = GLProgramCache::getInstance()->getGLProgram( "cocos_research_render_SpriteBatchNodeAlphaScene" );

					//
					// Create GLProgramState
					//
					auto gl_program_state = GLProgramState::getOrCreateWithGLProgram( gl_program );

					//
					// Apply
					//
					mTileMapNode->setGLProgramState( gl_program_state );
				}
			}

			//
			// Pivot
			//
			{
				auto pivot_node = cpg_node::PivotNode::create();
				pivot_node->setPosition( mTileMapNode->getPosition() );
				addChild( pivot_node, std::numeric_limits<int>::max() );
			}

		}

		//
		//
		//
		mTileMapOpacityLabel->setString( std::to_string( mTileMapNode->getOpacity() ) );

		return true;
	}

	void SpriteBatchNodeScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpriteBatchNodeScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void SpriteBatchNodeScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void SpriteBatchNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		case EventKeyboard::KeyCode::KEY_W:
			mTileMapNode->setOpacity( cpg::clamp( static_cast<int>( mTileMapNode->getOpacity() ) + 10, 0, 255 ) );
			mTileMapOpacityLabel->setString( std::to_string( mTileMapNode->getOpacity() ) );
			return;
		case EventKeyboard::KeyCode::KEY_S:
			mTileMapNode->setOpacity( cpg::clamp( static_cast<int>( mTileMapNode->getOpacity() ) - 10, 0, 255 ) );
			mTileMapOpacityLabel->setString( std::to_string( mTileMapNode->getOpacity() ) );
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
