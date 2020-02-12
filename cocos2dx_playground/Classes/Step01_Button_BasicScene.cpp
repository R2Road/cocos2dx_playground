#include "Step01_Button_BasicScene.h"

#include <new>
#include <sstream>

#include "ui/UIButton.h"

#include "Step01_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_label = 20140416;
}

namespace step01
{
	namespace button
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr )
		{}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

		bool BasicScene::init()
		{
			if( !Scene::init() )
				return false;

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
				ss << "[ESC] : Return to Step01 Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// Button
			//
			{
				auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( Size( 50.f, 50.f ) );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				button->addTouchEventListener( CC_CALLBACK_2( BasicScene::onButton, this ) );
				addChild( button, 0 );

				auto label = Label::createWithTTF( "Click Here ===>>>", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 1.f, 0.5f ) );
				label->setPosition( button->getPosition() - Vec2( button->getContentSize().width * 0.7f , 0.f ) );
				addChild( label, 1 );
			}

			//
			// Button Status View
			//
			{
				auto label = Label::createWithTTF( "Release", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_label );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				addChild( label, 1 );
			}

			

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void BasicScene::onButton( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto label = static_cast<Label*>( getChildByTag( TAG_label ) );
				label->setString( "Press" );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto label = static_cast<Label*>( getChildByTag( TAG_label ) );
				label->setString( "Move" );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				auto label = static_cast<Label*>( getChildByTag( TAG_label ) );
				label->setString( "Release" );
			}
			else if( ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				auto label = static_cast<Label*>( getChildByTag( TAG_label ) );
				label->setString( "Release( Cancel )" );
			}
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}
		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				return;
			}

			if( isScheduled( schedule_selector( BasicScene::updateForExit ) ) )
			{
				return;
			}

			scheduleOnce( schedule_selector( BasicScene::updateForExit ), 0.f );
		}
	}
}
