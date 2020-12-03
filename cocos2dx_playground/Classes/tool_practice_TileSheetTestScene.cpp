#include "tool_practice_TileSheetTestScene.h"

#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "cpg_StringTable.h"
#include "cpgui_ToolBarNode.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileMapNode.h"
#include "step_defender_tool_TileSheetNode.h"

USING_NS_CC;

namespace
{
	const int TAG_ToolBar = 20140416;

	const step_defender::game::TileSheetConfiguration ToolPractice_TileSheetConfig( 16, 16, 1, 1, "textures/tool_practice/tool_practice_tilesheettest.png" );

	const step_defender::game::TileMapNode::Config ToolPractice_TileMapConfig { 10, 10 };
}

namespace tool_practice
{
	TileSheetTestScene::TileSheetTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mGridIndexConverter( step_defender::game::TileSheetConfig.TileWidth, step_defender::game::TileSheetConfig.TileHeight )

		, mTileMapNode( nullptr )
			
		, mCurrentTilePoint( { 0, 0 } )
		, mToolIndex( 0 )
	{}

	Scene* TileSheetTestScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) TileSheetTestScene( back_to_the_previous_scene_callback );
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

	bool TileSheetTestScene::init()
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
		// Summury
		//
		{
			std::stringstream ss;
			ss << "[ESC] : Return to Root";
			ss << std::endl;
			ss << std::endl;
			ss << "[W/E] : " << "Shortcut( Pick/Erase )";

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
			auto layer = LayerColor::create( Color4B::BLACK );
			addChild( layer, std::numeric_limits<int>::min() );
		}

		//
		// Tool Selector
		//
		{
			auto tool_bar_node = cpgui::ToolBarNode::create( ui::Layout::Type::VERTICAL );
			tool_bar_node->setTag( TAG_ToolBar );
			addChild( tool_bar_node, std::numeric_limits<int>::max() );

			tool_bar_node->AddTool( 0, "step_defender_tool_icon_01_0.png", std::bind( &TileSheetTestScene::onToolSelect, this, 0 ) );
			tool_bar_node->AddTool( 1, "step_defender_tool_icon_01_1.png", std::bind( &TileSheetTestScene::onToolSelect, this, 1 ) );

			tool_bar_node->setPosition(
				visibleOrigin.x + visibleSize.width - tool_bar_node->getContentSize().width
				, visibleOrigin.y + visibleSize.height - tool_bar_node->getContentSize().height
			);

			// Set Indicator
			tool_bar_node->SelectTool( mToolIndex );
		}

		//
		// Tile Sheet Node
		//
		{
			auto tile_sheet_node = step_defender::tool::TileSheetNode::create( ToolPractice_TileSheetConfig );
			tile_sheet_node->setPosition(
				Vec2( visibleOrigin.x + visibleSize.width, visibleCenter.y )
				- Vec2( tile_sheet_node->getContentSize().width + 10.f, tile_sheet_node->getContentSize().height * 0.5f )
			);
			tile_sheet_node->SetSelectCallback( CC_CALLBACK_2( TileSheetTestScene::onTileSelect, this ) );
			addChild( tile_sheet_node, std::numeric_limits<int>::max() );
		}

		//
		// Tile Edit Node
		//
		{
			auto root_node = LayerColor::create( Color4B::GRAY );
			addChild( root_node, 0 );
			{
				// Tile Map
				{
					mTileMapNode = step_defender::game::TileMapNode::create(
						ToolPractice_TileMapConfig
						, ToolPractice_TileSheetConfig
					);
					mTileMapNode->setPosition( 4.f, 4.f );
					root_node->addChild( mTileMapNode );

					root_node->setContentSize( mTileMapNode->getContentSize() + Size( 8.f, 8.f ) );
					root_node->setPosition(
						visibleOrigin
						+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.5f )
						- Vec2( root_node->getContentSize().width * 0.5f, root_node->getContentSize().height * 0.5f )
					);
				}

				//
				// Touch Node
				//
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_4.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->setAnchorPoint( Vec2::ZERO );
					button->setScale9Enabled( true );
					button->setContentSize( root_node->getContentSize() );
					button->addTouchEventListener( CC_CALLBACK_2( TileSheetTestScene::onUpdateTile, this ) );
					root_node->addChild( button, std::numeric_limits<int>::max() - 1 );
				}
			}
		}

		return true;
	}

	void TileSheetTestScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TileSheetTestScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void TileSheetTestScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void TileSheetTestScene::onToolSelect( const int tool_index )
	{
		CCLOG( "%d", tool_index );
		mToolIndex = tool_index;
	}
	void TileSheetTestScene::onTileSelect( const int x, const int y )
	{
		mCurrentTilePoint.x = x;
		mCurrentTilePoint.y = y;
	}


	void TileSheetTestScene::onUpdateTile( Ref* sender, ui::Widget::TouchEventType touch_event_type )
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

		const auto point = mGridIndexConverter.Position2Point( pos.x, pos.y );
		CCLOG( "A : %d, %d", point.x, point.y );

		if( 0 > point.x || ToolPractice_TileMapConfig.MapWidth <= point.x || 0 > point.y || ToolPractice_TileMapConfig.MapHeight <= point.y )
		{
			return;
		}

		if( 0 == mToolIndex )
		{
			onAddTile( point.x, point.y );
		}
		else
		{
			onEraseTile( point.x, point.y );
		}
	}
	void TileSheetTestScene::onAddTile( const int grid_point_x, const int grid_point_y )
	{
		mTileMapNode->UpdateTile( grid_point_x, grid_point_y, mCurrentTilePoint.x, mCurrentTilePoint.y );
	}
	void TileSheetTestScene::onEraseTile( const int grid_point_x, const int grid_point_y )
	{
		mTileMapNode->EraseTile( grid_point_x, grid_point_y );
	}


	void TileSheetTestScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}

		if( EventKeyboard::KeyCode::KEY_W == key_code )
		{
			onToolSelect( 0 );
			static_cast<cpgui::ToolBarNode*>( getChildByTag( TAG_ToolBar ) )->SelectTool( mToolIndex );
		}
		if( EventKeyboard::KeyCode::KEY_E == key_code )
		{
			onToolSelect( 1 );
			static_cast<cpgui::ToolBarNode*>( getChildByTag( TAG_ToolBar ) )->SelectTool( mToolIndex );
		}
	}
}
