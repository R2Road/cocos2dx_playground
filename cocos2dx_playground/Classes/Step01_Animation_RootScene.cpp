#include "Step01_Animation_RootScene.h"

#include <new>
#include <sstream>

#include "step_clickclick_RootScene.h"
#include "Step01_Animation_BasicScene.h"
#include "Step01_Animation_PlayNStopScene.h"

USING_NS_CC;

namespace step01
{
	namespace animation
	{
		RootScene::RootScene() : mInputBlocked( false ), mKeyboardListener( nullptr ) {}

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
				ss << "+ Animation Root";
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Step 01 Root";
				ss << "\n";
				ss << "\n";
				ss << "[1] : Basic";
				ss << "\n";
				ss << "[2] : Play And Stop";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( label, 9999 );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 41, 0, 72, 255 ) );
				addChild( background_layer, 0 );
			}

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

		void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( mInputBlocked )
			{
				return;
			}

			Scene* next_scene = nullptr;
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				next_scene = step_clickclick::RootScene::create();
				break;

			case EventKeyboard::KeyCode::KEY_1:
				next_scene = BasicScene::create();
				break;
			case EventKeyboard::KeyCode::KEY_2:
				next_scene = PlayNStopScene::create();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}

			if( next_scene )
			{
				mInputBlocked = true;
				Director::getInstance()->replaceScene( next_scene );
			}
		}
	}
}
