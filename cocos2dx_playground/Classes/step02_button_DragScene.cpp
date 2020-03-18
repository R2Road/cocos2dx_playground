#include "step02_button_DragScene.h"

#include <new>
#include <sstream>

#include "ui/UIButton.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_rain_of_chaos_RootScene.h"

USING_NS_CC;

const int TAG_Button = 20140416;

namespace step02
{
	namespace button
	{
		DragScene::DragScene() : mKeyboardListener( nullptr )
		{}

		Scene* DragScene::create()
		{
			auto ret = new ( std::nothrow ) DragScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool DragScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[Mouse] : Do Click and Drag";
				ss << std::endl;
				ss << "[1] : Position Reset";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 15, 49, 101, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Button
			//
			{
				auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				button->setTag( TAG_Button );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				button->addTouchEventListener( CC_CALLBACK_2( DragScene::onButton, this ) );
				addChild( button, 100 );

				// left label
				{
					auto label = Label::createWithTTF( "CLICK HERE ===>>>", "fonts/arial.ttf", 9 );
					label->setColor( Color3B::RED );
					label->setAnchorPoint( Vec2( 1.f, 0.5f ) );
					label->setPosition(
						-4.f
						, button->getContentSize().height * 0.5f
					);
					button->addChild( label );
				}

				// right label
				{
					auto label = Label::createWithTTF( "<<<=== ...AND DRAG", "fonts/arial.ttf", 9 );
					label->setColor( Color3B::RED );
					label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
					label->setPosition(
						button->getContentSize().width + 4.f
						, button->getContentSize().height * 0.5f
					);
					button->addChild( label );
				}
			}

			return true;
		}

		void DragScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( DragScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void DragScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Node::onExit();
		}

		void DragScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mButtonMoveOffset = button->getPosition() - button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				button->setPosition( button->getTouchMovePosition() + mButtonMoveOffset );
			}
		}

		void DragScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
		}
		void DragScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( DragScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( DragScene::updateForExit ), 0.f );
				}
				return;
			}

			if( EventKeyboard::KeyCode::KEY_1 == keycode )
			{
				const auto visibleSize = Director::getInstance()->getVisibleSize();
				const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

				auto node = getChildByTag( TAG_Button );
				node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
			}
		}
	}
}
