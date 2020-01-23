#include "Research_Input_RootScene.h"

#include <new>
#include <sstream>

#include "Step01_RootScene.h"

#include "Research_Input_BasicScene.h"
#include "Research_Input_KeyCodeViewScene.h"

USING_NS_CC;

namespace research
{
	namespace input
	{
		RootScene::RootScene() : mKeyboardListener( nullptr ) {}

		Scene* RootScene::create()
		{
			auto ret = new ( std::nothrow ) RootScene();
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

		bool RootScene::init()
		{
			if( !Scene::init() )
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto origin = Director::getInstance()->getVisibleOrigin();

			//
			// Summury
			//
			std::stringstream ss;
			ss << "+ Input Research Root";
			ss << "\n";
			ss << "\n";
			ss << "[ESC] : Return to Root";
			ss << "\n";
			ss << "\n";
			ss << "[1] : Basic";
			ss << "\n";
			ss << "[2] : Key Code View";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
			label->setPosition( Vec2(
				origin.x + visibleSize.width * 0.5f
				, origin.y + visibleSize.height * 0.5f
			) );
			addChild( label, 9999 );

			return true;
		}

		void RootScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void RootScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void RootScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step01::RootScene::create() );
		}

		void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( RootScene::updateForExit ) ) )
					scheduleOnce( schedule_selector( RootScene::updateForExit ), 0.f );
				break;

			case EventKeyboard::KeyCode::KEY_1:
				Director::getInstance()->replaceScene( research::input::BasicScene::create() );
				break;

			case EventKeyboard::KeyCode::KEY_2:
				Director::getInstance()->replaceScene( research::input::KeyCodeViewScene::create() );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
