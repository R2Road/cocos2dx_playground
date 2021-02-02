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
#include "algorithm_practice_floodfill_EditorNode.h"

#include "cpg_StringTable.h"
#include "cpgui_ToolBarNode.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileMapNode.h"

USING_NS_CC;

namespace
{
	const char* FileName = "algorithm_practice_floodfill_test_scene.json";

	const std::size_t GRID_WIDTH = 13;
	const std::size_t GRID_HEIGHT = 13;

	const int TAG_ToolBar = 20140416;
}

namespace algorithm_practice_floodfill
{
	TestScene::TestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )

		, mMode( eMode::Edit )

		, mTileSheetConfiguration( 1, 1, 1, 1, "" )
		, mGrid4TileMap()

		, mTileMapNode( nullptr )
		, mEntryPointIndicatorNode( nullptr )
		, mDirectionMapNode( nullptr )
		, mEditorNode( nullptr )

		, mStep( eStep::Entry )
		, mGrid4FloodFill()
		, mCurrentPoint()
		, mCurrentPointIndicatorNode( nullptr )
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
		// Summury
		//
		{
			std::stringstream ss;
			ss << "[ESC] : Return to Root";
			ss << std::endl;
			ss << std::endl;
			ss << "[Mouse] : " << "Edit Grid";
			ss << std::endl;
			ss << std::endl;
			ss << "[R] : " << "Reset";
			ss << std::endl;
			ss << "[Space] : " << "Step";

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
			// Load TileMap Grid
			std::string file_path;
			file_path = FileUtils::getInstance()->getWritablePath();
			file_path += FileName;

			const std::string json_string( FileUtils::getInstance()->getStringFromFile( file_path ) );

			if( !mGrid4TileMap.LoadJsonString( json_string ) )
			{
				mGrid4TileMap.Reset( GRID_WIDTH, GRID_HEIGHT );
			}

			// Setup Direction Grid
			mGrid4FloodFill.Reset( mGrid4TileMap.GetWidth(), mGrid4TileMap.GetHeight() );
		}

		//
		// Tool Bar - for Mode
		//
		{
			auto tool_bar_node = cpgui::ToolBarNode::create();
			addChild( tool_bar_node, std::numeric_limits<int>::max() );

			tool_bar_node->AddTool( eToolIndex::Wall, "E", 10, std::bind( &TestScene::onModeSelect, this, eMode::Edit ) );
			tool_bar_node->AddTool( eToolIndex::Road, "P", 10, std::bind( &TestScene::onModeSelect, this, eMode::Process ) );

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
				, mTileSheetConfiguration
			);
			mTileMapNode->setPosition(
				visibleCenter
				- Vec2( mTileMapNode->getContentSize().width * 0.5f, mTileMapNode->getContentSize().height * 0.5f )
			);
			addChild( mTileMapNode );
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
		// Editor Node
		//
		{
			mEditorNode = EditorNode::create( { GRID_WIDTH, GRID_HEIGHT }, &mGrid4TileMap, mTileMapNode, mEntryPointIndicatorNode, mTileSheetConfiguration );
			addChild( mEditorNode, 1 );
		}

		//
		// Current Point Indicator
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
			sprite->setTextureRect( tile_sheet_utility.ConvertTilePoint2TextureRect( 0, 4 ) );
			sprite->setVisible( false );
			addChild( sprite, 11 );

			mCurrentPointIndicatorNode = sprite;
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

			mStep = eStep::Entry;

			for( auto& d : mGrid4FloodFill )
			{
				d.Clear();
			}
			mDirectionMapNode->Reset();

			mCurrentPointIndicatorNode->setVisible( false );
		}
		else
		{
			mEditorNode->setVisible( false );
		}
	}


	void TestScene::updateCurrentPointView()
	{
		mCurrentPointIndicatorNode->setPosition(
			mTileMapNode->getPosition()
			+ Vec2( mTileSheetConfiguration.GetTileWidth() * mCurrentPoint.x, mTileSheetConfiguration.GetTileHeight() * mCurrentPoint.y )
		);
	}


	void TestScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}

		if( eMode::Process == mMode )
		{
			if( EventKeyboard::KeyCode::KEY_R == key_code )
			{
				mStep = eStep::Entry;
				for( auto& d : mGrid4FloodFill )
				{
					d.Clear();
				}
				mDirectionMapNode->Reset();

				mCurrentPointIndicatorNode->setVisible( false );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == key_code )
			{
				if( eStep::Entry == mStep )
				{
					mStep = eStep::Loop;
					auto& current_cell = mGrid4FloodFill.Get( mGrid4TileMap.GetEntryPoint().x, mGrid4TileMap.GetEntryPoint().y );
					current_cell.Begin( { -1, -1 }, cpg::Direction4::eState::None );
					mDirectionMapNode->UpdateTile( mGrid4TileMap.GetEntryPoint().x, mGrid4TileMap.GetEntryPoint().y, current_cell.GetTotalDirection() );

					mCurrentPoint = mGrid4TileMap.GetEntryPoint();
					mCurrentPointIndicatorNode->setVisible( true );
					updateCurrentPointView();
				}
				else if( eStep::Loop == mStep )
				{
					auto& current_cell = mGrid4FloodFill.Get( mCurrentPoint.x, mCurrentPoint.y );
					if( current_cell.HasDirection() )
					{
						const auto current_direction = current_cell.PopDirection();
						mDirectionMapNode->UpdateTile( mCurrentPoint.x, mCurrentPoint.y, current_cell.GetTotalDirection() );

						auto new_point = mCurrentPoint + current_direction.GetPoint();
						if( mGrid4FloodFill.Get( new_point.x, new_point.y ).IsValid() )
						{
							return;
						}

						if( !mGrid4FloodFill.IsIn( new_point.x, new_point.y ) )
						{
							return;
						}

						if( eCellType::Road == mGrid4TileMap.GetCellType( new_point.x, new_point.y ) )
						{
							auto& next_cell = mGrid4FloodFill.Get( new_point.x, new_point.y );
							next_cell.Begin( mCurrentPoint, current_direction );
							mDirectionMapNode->UpdateTile( new_point.x, new_point.y, next_cell.GetTotalDirection() );

							mCurrentPoint = new_point;
							updateCurrentPointView();
						}
					}
					else
					{
						mCurrentPoint = current_cell.GetParentPoint();
						updateCurrentPointView();

						if( -1 == mCurrentPoint.x )
						{
							mStep = eStep::End;
						}
					}
				}
			}
		}
	}
}
