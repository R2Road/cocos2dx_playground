#include "algorithm_practice_astar_TestScene.h"

#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "cocos/platform/CCFileUtils.h"

#include "algorithm_practice_astar_EditorNode.h"
#include "algorithm_practice_astar_ProcessorNode.h"

#include "cpg_StringTable.h"
#include "cpg_TileSheetConfiguration.h"
#include "cpg_ui_ToolBarNode.h"

#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace
{
	const char* FileName = "algorithm_practice_astar_test_scene.json";

	const std::size_t GRID_WIDTH = 13;
	const std::size_t GRID_HEIGHT = 13;
}

namespace algorithm_practice_astar
{
	TestScene::TestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mMode( eMode::Edit )

		, mGrid4TileMap()

		, mTileMapNode( nullptr )
		, mEntryPointIndicatorNode( nullptr )
		, mExitPointIndicatorNode( nullptr )
		, mEditorNode( nullptr )
		, mProcessorNode( nullptr )
	{}

	Scene* TestScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) TestScene( back_to_the_previous_scene_callback );
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

	bool TestScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleOrigin = _director->getVisibleOrigin();
		const auto visibleSize = _director->getVisibleSize();
		const Vec2 visibleCenter(
			visibleOrigin.x + ( visibleSize.width * 0.5f )
			, visibleOrigin.y + ( visibleSize.height * 0.5f )
		);

		//
		// Load Tile Config
		//
		cpg::TileSheetConfiguration tile_sheet_configuration;
		CCASSERT( tile_sheet_configuration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "[ESC] : Return to Root";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7 );
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
			auto layer = LayerColor::create( Color4B( 8, 45, 48, 255 ) );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		//
		// Load Grid
		//
		{
			// Load TileMap Grid
			std::string file_path;
			file_path = FileUtils::getInstance()->getWritablePath();
			file_path += FileName;

			const std::string json_string( FileUtils::getInstance()->getStringFromFile( file_path ) );

			if( !mGrid4TileMap.LoadJsonString( json_string ) )
			{
				mGrid4TileMap.Reset( GRID_WIDTH, GRID_HEIGHT );
			}
		}

		//
		// Tool Bar - for Mode
		//
		{
			auto tool_bar_node = cpg_ui::ToolBarNode::create( ui::Layout::Type::VERTICAL, Size( 60.f, 30.f ) );
			addChild( tool_bar_node, std::numeric_limits<int>::max() );

			tool_bar_node->AddTool( eMode::Edit, "Editor", 10, std::bind( &TestScene::onModeSelect, this, eMode::Edit ) );
			tool_bar_node->AddTool( eMode::Process, "Processor", 10, std::bind( &TestScene::onModeSelect, this, eMode::Process ) );

			tool_bar_node->setPosition(
				visibleOrigin
				+ Vec2( 0.f, visibleSize.height * 0.5f )
			);

			// Set Indicator
			tool_bar_node->SelectTool( mMode );
		}

		//
		// Tile Maps
		//
		{
			mTileMapNode = step_defender::game::TileMapNode::create(
				step_defender::game::TileMapNode::Config{ GRID_WIDTH, GRID_HEIGHT }
				, tile_sheet_configuration
			);
			mTileMapNode->setPosition(
				visibleCenter
				- Vec2( mTileMapNode->getContentSize().width * 0.5f, mTileMapNode->getContentSize().height * 0.5f )
			);
			addChild( mTileMapNode );
		}

		//
		// Entry N Exit Point Indicator
		//
		{
			{
				auto draw_node = DrawNode::create();
				draw_node->drawRect( Vec2( 1.f, 1.f ), Vec2( tile_sheet_configuration.GetTileWidth() - 1, tile_sheet_configuration.GetTileHeight() - 1 ), Color4F( Color3B( 255u, 97u, 178u ), 1.f ) );
				addChild( draw_node, 10 );

				mEntryPointIndicatorNode = draw_node;
			}

			{
				auto draw_node = DrawNode::create();
				draw_node->drawRect( Vec2( 1.f, 1.f ), Vec2( tile_sheet_configuration.GetTileWidth() - 1, tile_sheet_configuration.GetTileHeight() - 1 ), Color4F( Color3B( 162u, 255u, 203u ), 1.f ) );
				addChild( draw_node, 10 );

				mExitPointIndicatorNode = draw_node;
			}
		}

		//
		// Editor Node
		//
		{
			mEditorNode = algorithm_practice_astar::EditorNode::create(
				{ GRID_WIDTH, GRID_HEIGHT }
				, &mGrid4TileMap
				, mTileMapNode
				, mEntryPointIndicatorNode
				, mExitPointIndicatorNode
				, tile_sheet_configuration
			);
			addChild( mEditorNode, 1 );
		}		

		//
		// Processor Node
		//
		{
			mProcessorNode = algorithm_practice_astar::ProcessorNode::create( { GRID_WIDTH, GRID_HEIGHT }, tile_sheet_configuration, &mGrid4TileMap );
			addChild( mProcessorNode, 2 );
		}

		//
		// Setup
		//
		onModeSelect( mMode );

		return true;
	}

	void TestScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TestScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void TestScene::onExit()
	{
		//
		// Save
		//
		{
			std::string json_string;
			mGrid4TileMap.ExportJsonString( json_string );

			std::string file_path = FileUtils::getInstance()->getWritablePath();
			file_path += FileName;

			CCASSERT( FileUtils::getInstance()->writeStringToFile( json_string, file_path ), "Failed : Terrain Data Save" );
		}

		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void TestScene::onModeSelect( const int mode_index )
	{
		mMode = static_cast<eMode>( mode_index );
		CCLOG( "Mode Index : %d", mode_index );

		if( eMode::Edit == mMode )
		{
			mEditorNode->setVisible( true );
			mProcessorNode->setVisible( false );
		}
		else
		{
			mEditorNode->setVisible( false );
			mProcessorNode->setVisible( true );
		}
	}


	void TestScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
