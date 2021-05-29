#include "tool_practice_TileSheetTestScene.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIButton.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "cpg_ui_ToolBarNode.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileMapNode.h"
#include "step_defender_tool_TileSheetNode.h"

USING_NS_CC;

namespace
{
	const int TAG_LayerSelector = 100;
	const int TAG_ToolSelector = 20140416;
}

namespace tool_practice
{
	TileSheetTestScene::TileSheetTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mConfiguration()

		, mPosition2GridIndexConverter( 1, 1 )

		, mTileMapNode( nullptr )
			
		, mCurrentTilePoint( { 0, 0 } )
		, mToolIndex( eToolIndex::Pick )
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

		if( !mConfiguration.Load( "datas/tool_practice/tile_sheet_test.json" ) )
		{
			return false;
		}

		//
		// Setup Position 2 Grid Index Converter
		//
		mPosition2GridIndexConverter = cpg::Position2GridIndexConverter(
			mConfiguration.GetTileSheetConfiguration().GetTileWidth()
			, mConfiguration.GetTileSheetConfiguration().GetTileHeight()
		);

		//
		// Reload Texture
		//
		reloadTexture();

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
			ss << cpg::linefeed;
			ss << cpg::linefeed;

			ss << "[";
			for( int i = 0; mConfiguration.GetLayerCount() > i; ++i )
			{
				ss << std::to_string( i + 1 ).c_str();

				if( mConfiguration.GetLayerCount() > i + 1 )
				{
					ss << "/";
				}
			}
			ss << "] : Layer Select";

			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[B/E] : " << "Shortcut( Pick/Erase )";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[R] : " << "Reload Texture";

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
		// Layer Selector
		//
		{
			auto tool_bar_node = cpg_ui::ToolBarNode::create();
			tool_bar_node->setTag( TAG_LayerSelector );
			addChild( tool_bar_node, std::numeric_limits<int>::max() );

			for( int i = 0; mConfiguration.GetLayerCount() > i; ++i )
			{
				tool_bar_node->AddTool( i, std::to_string( i + 1 ).c_str(), 10, std::bind( &TileSheetTestScene::onLayerSelect, this, i ) );
			}

			tool_bar_node->setPosition(
				visibleCenter.x - ( tool_bar_node->getContentSize().width * 0.5f )
				, visibleOrigin.y + visibleSize.height - tool_bar_node->getContentSize().height
			);

			// Set Indicator
			tool_bar_node->SelectTool( 0 );
		}

		//
		// Tool Selector
		//
		{
			auto tool_bar_node = cpg_ui::ToolBarNode::create( ui::Layout::Type::VERTICAL );
			tool_bar_node->setTag( TAG_ToolSelector );
			addChild( tool_bar_node, std::numeric_limits<int>::max() );

			tool_bar_node->AddTool( 0, "step_defender_tool_icon_01_0.png", std::bind( &TileSheetTestScene::onToolSelect, this, static_cast<int>( eToolIndex::Pick ) ) );
			tool_bar_node->AddTool( 1, "step_defender_tool_icon_01_1.png", std::bind( &TileSheetTestScene::onToolSelect, this, static_cast<int>( eToolIndex::Erase ) ) );

			tool_bar_node->setPosition(
				visibleOrigin.x + visibleSize.width - tool_bar_node->getContentSize().width
				, visibleOrigin.y + visibleSize.height - tool_bar_node->getContentSize().height
			);

			// Set Indicator
			tool_bar_node->SelectTool( static_cast<int>( mToolIndex ) );
		}

		//
		// Tile Sheet Node
		//
		{
			auto tile_sheet_node = step_defender::tool::TileSheetNode::create( mConfiguration.GetTileSheetConfiguration() );
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
				// Tile Maps
				for( int i = 0; mConfiguration.GetLayerCount() > i; ++i )
				{
					auto tile_map_node = step_defender::game::TileMapNode::create(
						step_defender::game::TileMapNode::Config{ mConfiguration.GetWidth(), mConfiguration.GetHeight() }
						, mConfiguration.GetTileSheetConfiguration()
					);
					tile_map_node->setTag( i );
					tile_map_node->setPosition( 4.f, 4.f );
					root_node->addChild( tile_map_node );

					mTileMapNodeContainer.push_back( tile_map_node );
				}

				root_node->setContentSize( ( *mTileMapNodeContainer.begin() )->getContentSize() + Size( 8.f, 8.f ) );
				root_node->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.5f )
					- Vec2( root_node->getContentSize().width * 0.5f, root_node->getContentSize().height * 0.5f )
				);

				//
				// Touch Node
				//
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_4.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->setAnchorPoint( Vec2::ZERO );
					button->setScale9Enabled( true );
					button->setContentSize( root_node->getContentSize() );
					button->addTouchEventListener( CC_CALLBACK_2( TileSheetTestScene::onUpdateTile, this ) );
					root_node->addChild( button, std::numeric_limits<int>::max() );
				}
			}
		}

		//
		// Setup
		//
		mTileMapNode = mTileMapNodeContainer[0u];

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


	void TileSheetTestScene::reloadTexture()
	{
		_director->getTextureCache()->reloadTexture( mConfiguration.GetTileSheetConfiguration().GetTexturePath() );
	}


	void TileSheetTestScene::onLayerSelect( const int layer_index )
	{
		CCASSERT( 0 <= layer_index && layer_index < static_cast<int>( mTileMapNodeContainer.size() ), "Invalid Layer Index" );
		mTileMapNode = mTileMapNodeContainer[layer_index];

		for( auto n : mTileMapNodeContainer )
		{
			n->setVisible( n->getTag() <= layer_index );
		}
	}
	void TileSheetTestScene::onToolSelect( const int tool_index )
	{
		CCLOG( "%d", tool_index );
		mToolIndex = static_cast<eToolIndex>( tool_index );
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

		const auto point = mPosition2GridIndexConverter.Position2Point( pos.x, pos.y );
		CCLOG( "A : %d, %d", point.x, point.y );

		if( 0 > point.x || mConfiguration.GetWidth() <= point.x || 0 > point.y || mConfiguration.GetHeight() <= point.y )
		{
			return;
		}

		if( eToolIndex::Pick == mToolIndex )
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

		if( EventKeyboard::KeyCode::KEY_1 <= key_code && EventKeyboard::KeyCode::KEY_9 >= key_code )
		{
			const int target_layer_index = static_cast<int>( key_code ) - static_cast<int>( EventKeyboard::KeyCode::KEY_1 );
			if( target_layer_index < mConfiguration.GetLayerCount() )
			{
				onLayerSelect( target_layer_index );
				static_cast<cpg_ui::ToolBarNode*>( getChildByTag( TAG_LayerSelector ) )->SelectTool( target_layer_index );
			}

			return;
		}

		switch( key_code )
		{
		case EventKeyboard::KeyCode::KEY_B:
			onToolSelect( static_cast<int>( eToolIndex::Pick ) );
			static_cast<cpg_ui::ToolBarNode*>( getChildByTag( TAG_ToolSelector ) )->SelectTool( static_cast<int>( eToolIndex::Pick ) );
			break;
		case EventKeyboard::KeyCode::KEY_E:
			onToolSelect( static_cast<int>( eToolIndex::Erase ) );
			static_cast<cpg_ui::ToolBarNode*>( getChildByTag( TAG_ToolSelector ) )->SelectTool( static_cast<int>( eToolIndex::Erase ) );
			break;

		case EventKeyboard::KeyCode::KEY_R :
			reloadTexture();
			break;
		}
	}
}
