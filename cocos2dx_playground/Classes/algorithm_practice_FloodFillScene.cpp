#include "algorithm_practice_FloodFillScene.h"

#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIButton.h"

#include "cpg_StringTable.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileMapNode.h"
#include "step_defender_tool_TileSheetNode.h"

USING_NS_CC;

namespace
{
	const int TAG_ToolBar = 20140416;
}

namespace algorithm_practice
{
	FloodFillScene::FloodFillScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mConfiguration()

		, mPosition2GridIndexConverter( 1, 1 )

		, mTileMapNode( nullptr )
	{}

	Scene* FloodFillScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) FloodFillScene( back_to_the_previous_scene_callback );
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

	bool FloodFillScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		if( !mConfiguration.Load() )
		{
			return false;
		}

		//
		// Setup Grid Index Converter
		//
		mPosition2GridIndexConverter = cpg::Position2GridIndexConverter( mConfiguration.GetTileSheetConfiguration().TileWidth, mConfiguration.GetTileSheetConfiguration().TileHeight );

		//
		// Reload Texture
		//
		_director->getTextureCache()->reloadTexture( mConfiguration.GetTileSheetConfiguration().TexturePath );

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
			ss << "[Mouse] : " << "Select Pivot";

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
		// Tile Edit Node
		//
		{
			auto root_node = LayerColor::create( Color4B::GRAY );
			addChild( root_node, 0 );
			{
				// Tile Maps
				{
					mTileMapNode = step_defender::game::TileMapNode::create(
						step_defender::game::TileMapNode::Config{ mConfiguration.GetWidth(), mConfiguration.GetHeight() }
						, mConfiguration.GetTileSheetConfiguration()
					);
					mTileMapNode->setPosition( 4.f, 4.f );
					root_node->addChild( mTileMapNode );
				}

				root_node->setContentSize( mTileMapNode->getContentSize() + Size( 8.f, 8.f ) );
				root_node->setPosition(
					visibleCenter
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
					button->addTouchEventListener( CC_CALLBACK_2( FloodFillScene::onUpdateTile, this ) );
					root_node->addChild( button, std::numeric_limits<int>::max() );
				}
			}
		}

		return true;
	}

	void FloodFillScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( FloodFillScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void FloodFillScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void FloodFillScene::onUpdateTile( Ref* sender, ui::Widget::TouchEventType touch_event_type )
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

		// Do Something
	}


	void FloodFillScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
