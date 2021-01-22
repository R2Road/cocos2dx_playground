#include "algorithm_practice_floodfill_TestScene.h"

#include <functional>
#include <fstream>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "cocos/platform/CCFileUtils.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIButton.h"

#include "algorithm_practice_floodfill_DirectionMapNode.h"

#include "cpg_StringTable.h"
#include "cpgui_ToolBarNode.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace
{
	const char* FileName = "algorithm_practice_floodfill_test_scene.json";

	const std::size_t GRID_WIDTH = 12;
	const std::size_t GRID_HEIGHT = 12;

	const int TAG_ToolBar = 20140416;
}

namespace algorithm_practice_floodfill
{
	TestScene::TestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mTileSheetConfiguration( 1, 1, 1, 1, "" )

		, mPosition2GridIndexConverter( 1, 1 )

		, mGrid( GRID_WIDTH, GRID_HEIGHT )
		, mTileMapNode( nullptr )
		, mToolIndex( eToolIndex::Wall )
		, mEntryPoint()

		, mEntryPointIndicatorNode( nullptr )
		, mGridDebugViewNode( nullptr )

		, mDirectionMapNode( nullptr )
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
		CCASSERT( mTileSheetConfiguration.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

		//
		// Setup Grid Index Converter
		//
		mPosition2GridIndexConverter = cpg::Position2GridIndexConverter(
			mTileSheetConfiguration.GetTileWidth()
			, mTileSheetConfiguration.GetTileHeight()
		);

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "[ESC] : Return to Root";
			ss << std::endl;
			ss << std::endl;
			ss << "[Mouse] : " << "Edit Grid";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
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
			std::string file_path;
			file_path = FileUtils::getInstance()->getWritablePath();
			file_path += FileName;

			const std::string json_string( FileUtils::getInstance()->getStringFromFile( file_path ) );

			mGrid.LoadJsonString( json_string );
		}

		//
		// Tool Bar
		//
		{
			auto tool_bar_node = cpgui::ToolBarNode::create();
			addChild( tool_bar_node, std::numeric_limits<int>::max() );

			tool_bar_node->AddTool( eToolIndex::Wall, "W", 10, std::bind( &TestScene::onToolSelect, this, eToolIndex::Wall ) );
			tool_bar_node->AddTool( eToolIndex::Road, "R", 10, std::bind( &TestScene::onToolSelect, this, eToolIndex::Road ) );
			tool_bar_node->AddTool( eToolIndex::Entry, "E", 10, std::bind( &TestScene::onToolSelect, this, eToolIndex::Entry ) );

			tool_bar_node->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width, visibleSize.height )
				+ Vec2( -tool_bar_node->getContentSize().width, -tool_bar_node->getContentSize().height )
			);

			// Set Indicator
			tool_bar_node->SelectTool( mToolIndex );
		}

		//
		// Tile Maps
		//
		{
			mTileMapNode = step_defender::game::TileMapNode::create(
				step_defender::game::TileMapNode::Config{ GRID_WIDTH, GRID_HEIGHT }
				, mTileSheetConfiguration
			);
			mTileMapNode->setPosition(
				visibleCenter
				- Vec2( mTileMapNode->getContentSize().width * 0.5f, mTileMapNode->getContentSize().height * 0.5f )
			);
			addChild( mTileMapNode );
		}

		//
		// Direction Maps
		//
		{
			mDirectionMapNode = DirectionMapNode::create( DirectionMapNode::Config{ GRID_WIDTH, GRID_HEIGHT } );
			mDirectionMapNode->setPosition(
				visibleCenter
				- Vec2( mDirectionMapNode->getContentSize().width * 0.5f, mDirectionMapNode->getContentSize().height * 0.5f )
			);
			addChild( mDirectionMapNode );
		}

		//
		// Touch Node
		//
		{
			auto button = ui::Button::create( "guide_01_0.png", "guide_01_4.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
			button->setAnchorPoint( Vec2::ZERO );
			button->setScale9Enabled( true );
			button->setContentSize( mTileMapNode->getContentSize() + Size( 4.f, 4.f ) );
			button->setPosition(
				visibleCenter
				- Vec2( button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f )
			);
			button->addTouchEventListener( CC_CALLBACK_2( TestScene::onUpdateTile, this ) );
			addChild( button, std::numeric_limits<int>::max() );
		}

		//
		// Entry Point Indicator
		//
		{
			auto texture = Director::getInstance()->getTextureCache()->getTextureForKey( mTileSheetConfiguration.GetTexturePath() );

			cpg::TileSheetUtility tile_sheet_utility;
			tile_sheet_utility.Setup(
				mTileSheetConfiguration.GetTileWidth(), mTileSheetConfiguration.GetTileHeight()
				, mTileSheetConfiguration.GetTileMargin_Width(), mTileSheetConfiguration.GetTileMargin_Height()
				, texture->getContentSizeInPixels().height
			);

			auto sprite = Sprite::createWithTexture( texture );
			sprite->setAnchorPoint( Vec2::ZERO );
			sprite->setScale( _director->getContentScaleFactor() );
			sprite->setTextureRect( tile_sheet_utility.ConvertTilePoint2TextureRect( 0, 2 ) );
			addChild( sprite, 10 );

			mEntryPointIndicatorNode = sprite;
		}

		//
		// Grid Debug View Node
		//
		{
			cpg::TileSheetConfiguration tile_sheet_config( 1, 1, 1, 1, "" );
			CCASSERT( tile_sheet_config.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_02.json" ), "Failed - Load Tile Sheet Configuration" );

			mGridDebugViewNode = step_defender::game::TileMapNode::create(
				step_defender::game::TileMapNode::Config{ GRID_WIDTH, GRID_HEIGHT }
				, tile_sheet_config
			);
			mGridDebugViewNode->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height )
				- Vec2( mGridDebugViewNode->getContentSize().width * 0.5f, mGridDebugViewNode->getContentSize().height )
				- Vec2( 0.f, 2.f )
			);
			addChild( mGridDebugViewNode );
		}

		//
		// Setup
		//
		mTileMapNode->FillAll( 0, 0 );
		mGridDebugViewNode->FillAll( 0, 0 );

		mEntryPointIndicatorNode->setPosition(
			mTileMapNode->getPosition()
			+ Vec2( mTileSheetConfiguration.GetTileWidth() * mEntryPoint.x, mTileSheetConfiguration.GetTileHeight() * mEntryPoint.y )
		);

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
			mGrid.ExportJsonString( json_string );

			std::string file_path = FileUtils::getInstance()->getWritablePath();
			file_path += FileName;
			std::ofstream fs( file_path, std::ios::out );
			if( fs.fail() )
			{
				CCLOG( "Failed : Terrain Data Save" );
				return;
			}

			fs << json_string.c_str() << std::endl;
			fs.close();
		}

		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void TestScene::onToolSelect( const int tool_index )
	{
		mToolIndex = tool_index;
		CCLOG( "Tool Index : %d", mToolIndex );
	}
	void TestScene::onUpdateTile( Ref* sender, ui::Widget::TouchEventType touch_event_type )
	{
		auto button = static_cast<ui::Button*>( sender );

		Vec2 pos;
		if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
		{
			pos = mTileMapNode->convertToNodeSpace( button->getTouchBeganPosition() );
		}
		else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
		{
			pos = mTileMapNode->convertToNodeSpace( button->getTouchMovePosition() );
		}
		else //if( ui::Widget::TouchEventType::ENDED == touch_event_type || ui::Widget::TouchEventType::CANCELED == touch_event_type )
		{
			pos = mTileMapNode->convertToNodeSpace( button->getTouchEndPosition() );
		}

		const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
		CCLOG( "A : %d, %d", point.x, point.y );

		if( 0 > point.x || GRID_WIDTH <= point.x || 0 > point.y || GRID_HEIGHT <= point.y )
		{
			return;
		}

		//
		// Put Tile
		//
		switch( mToolIndex )
		{
		case eToolIndex::Wall:
			if( mEntryPoint != point )
			{
				mGrid.Set( point.x, point.y, GridValue{ eGridType::Wall } );
				mTileMapNode->UpdateTile( point.x, point.y, 1, 0 );

				onUpdateDebugView();
			}
			break;
		case eToolIndex::Road:
			if( mEntryPoint != point )
			{
				mGrid.Set( point.x, point.y, GridValue{ eGridType::Road } );
				mTileMapNode->UpdateTile( point.x, point.y, 0, 0 );

				onUpdateDebugView();
			}
			break;
		case eToolIndex::Entry:
			mEntryPoint = point;
			mGrid.Set( mEntryPoint.x, mEntryPoint.y, GridValue{ eGridType::Road } );
			mTileMapNode->UpdateTile( mEntryPoint.x, mEntryPoint.y, 0, 0 );
			
			mEntryPointIndicatorNode->setPosition(
				mTileMapNode->getPosition()
				+ Vec2( mTileSheetConfiguration.GetTileWidth() * mEntryPoint.x, mTileSheetConfiguration.GetTileHeight() * mEntryPoint.y )
			);

			onUpdateDebugView();
			break;

		default:
			CCASSERT( "Invalid Tool Index : %d", mToolIndex );
		}
	}
	void TestScene::onUpdateDebugView()
	{
		for( std::size_t y = 0; GRID_HEIGHT > y; ++y )
		{
			for( std::size_t x = 0; GRID_WIDTH > x; ++x )
			{
				if( eGridType::Road == mGrid.Get( x, y ).Type )
				{
					mGridDebugViewNode->UpdateTile( x, y, 0, 0 );
				}
				else
				{
					mGridDebugViewNode->UpdateTile( x, y, 1, 0 );
				}
			}
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
