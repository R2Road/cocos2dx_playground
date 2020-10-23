#include "step_defender_tool_parallax_layer_EntryScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"

USING_NS_CC;

namespace step_defender
{
	namespace tool_parallax_layer
	{
		EntryScene::EntryScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* EntryScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) EntryScene( back_to_the_previous_scene_callback );
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

		bool EntryScene::init()
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
			// Center Menu
			//
			{
				const Size MenuItemSize( 100.f, 30.f );
				auto MenuItemMargin = ui::Margin( 4.f, 2.f, 4.f, 2.f );
				const int MenuItemCount = 2;
				const Size MenuSize(
					MenuItemMargin.left + MenuItemSize.width + MenuItemMargin.right
					, ( MenuItemMargin.top + MenuItemSize.height + MenuItemMargin.bottom ) * MenuItemCount
				);

				auto layout_node = ui::Layout::create();
				layout_node->setContentSize( MenuSize );
				layout_node->setLayoutType( ui::Layout::Type::VERTICAL );
				layout_node->setClippingEnabled( true );
				layout_node->setBackGroundColor( Color3B::BLUE );
				layout_node->setBackGroundColorType( cocos2d::ui::Layout::BackGroundColorType::SOLID );
				layout_node->setPosition(
					visibleCenter
					- Vec2( layout_node->getContentSize().width * 0.5f, layout_node->getContentSize().height * 0.5f )
				);
				addChild( layout_node );

				for( int i = 0; MenuItemCount > i; ++i )
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->setTag( i );
					button->setScale9Enabled( true );
					button->setContentSize( MenuItemSize );
					//button->addTouchEventListener( CC_CALLBACK_2( EntryScene::onMenuButton, this ) );

					// Align
					{
						auto param = ui::LinearLayoutParameter::create();
						param->setMargin( MenuItemMargin );

						button->setLayoutParameter( param );
					}

					layout_node->addChild( button );
				}
			}

			return true;
		}

		void EntryScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( EntryScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void EntryScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void EntryScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == key_code )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
