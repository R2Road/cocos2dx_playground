#include "step_clickclick_button_BasicScene.h"

#include <new>
#include <sstream>

#include "ui/UIButton.h"

#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_ButtonStatus = 20140416;
	const int TAG_TouchPosition = 20160528;
}

namespace step_clickclick
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
				ss << "[ESC] : Return to Root";

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
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 0, 61, 33, 255 ) );
				addChild( background_layer, 0 );
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
				addChild( button, 1 );

				auto label = Label::createWithTTF( "Click Here ===>>>", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 1.f, 0.5f ) );
				label->setPosition( button->getPosition() - Vec2( button->getContentSize().width * 0.7f , 0.f ) );
				addChild( label, 2 );
			}

			//
			// Button Status View
			//
			{
				auto label = Label::createWithTTF( "Release", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_ButtonStatus );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				addChild( label, 2 );
			}

			//
			// Touch Info View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_TouchPosition );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.6f )
				) );
				addChild( label, 2 );
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

		void BasicScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				updateView_ButtonStatus( "Press" );
				updateView_TouchPosition( button->getTouchBeganPosition() );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				updateView_ButtonStatus( "Move" );
				updateView_TouchPosition( button->getTouchMovePosition() );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				updateView_ButtonStatus( "Release" );
				updateView_TouchPosition( button->getTouchEndPosition() );
			}
			else if( ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				updateView_ButtonStatus( "Release( Cancel )" );
			}
		}
		void BasicScene::updateView_ButtonStatus( const char* status_string )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ButtonStatus ) );
			label->setString( status_string );
		}
		void BasicScene::updateView_TouchPosition( const cocos2d::Vec2 touch_position )
		{
			auto touch_position_label = static_cast<Label*>( getChildByTag( TAG_TouchPosition ) );
			touch_position_label->setString( StringUtils::format( "x : %f, y : %f", touch_position.x, touch_position.y ) );
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
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
