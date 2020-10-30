#include "step_defender_tool_test_TileSheetNodeScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_defender_tool_TileSheetNode.h"

USING_NS_CC;

namespace step_defender
{
	namespace tool_test
	{
		TileSheetNodeScene::TileSheetNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* TileSheetNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) TileSheetNodeScene( back_to_the_previous_scene_callback );
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

		bool TileSheetNodeScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();
			const Vec2 visibleCenter( visibleOrigin.x + ( visibleSize.width * 0.5f ), visibleOrigin.y + ( visibleSize.height * 0.5f ) );

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 7, 39, 43, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Test Setup
			//
			{
				auto tile_sheet_node = step_defender::tool::TileSheetNode::create(
					step_defender::tool::TileSheetNode::Config{
						32 , 32
						, "textures/texture_001.png"
					}
				);
				tile_sheet_node->setPosition(
					visibleCenter
					- Size( tile_sheet_node->getContentSize().width * 0.5f, tile_sheet_node->getContentSize().height * 0.5f )
				);
				tile_sheet_node->SetSelectCallback( []( int x, int y )
				{
					CCLOG( "%d, %d", x, y );
				} );
				addChild( tile_sheet_node );
			}

			return true;
		}

		void TileSheetNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TileSheetNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TileSheetNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void TileSheetNodeScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
