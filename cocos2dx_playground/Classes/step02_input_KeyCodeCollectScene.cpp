#include "step02_input_KeyCodeCollectScene.h"

#include <new>
#include <sstream>

#include "CPG_Input_KeyCodeNames.h"
#include "step02_RootScene.h"

USING_NS_CC;

namespace step02
{
	namespace input
	{
		KeyCodeCollectScene::KeyCodeCollectScene() : mKeyboardListener( nullptr ), mKeyCodeCollector(), mKeyStrings( 100, 0 ), mKeyViewer( nullptr )
		{}

		Scene* KeyCodeCollectScene::create()
		{
			auto ret = new ( std::nothrow ) KeyCodeCollectScene();
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

		bool KeyCodeCollectScene::init()
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
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Root";
				ss << "\n";
				ss << "\n";
				ss << "[Keyboard] : Do Type";

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
				auto background_layer = LayerColor::create( Color4B( 5, 29, 81, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// key viewer
			//
			mKeyViewer = Label::createWithTTF( "", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
			mKeyViewer->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( mKeyViewer, 9999 );

			return true;
		}

		void KeyCodeCollectScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( KeyCodeCollectScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyCodeCollectScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void KeyCodeCollectScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Node::onExit();
		}

		void KeyCodeCollectScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step02::RootScene::create() );
		}

		void KeyCodeCollectScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( KeyCodeCollectScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( KeyCodeCollectScene::updateForExit ), 0.f );
				}
			}
			else
			{
				mKeyCodeCollector.onKeyPressed( keycode );
				updateKeyViewer();
			}
		}
		void KeyCodeCollectScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( keycode );
			updateKeyViewer();
		}

		void KeyCodeCollectScene::updateKeyViewer()
		{
			mKeyStrings.clear();
			for( auto cur = cpg::input::KeyCodeContainerFirst; cur < cpg::input::KeyCodeContainerSize; ++cur )
			{
				if( mKeyCodeCollector.isActiveKey( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) ) )
				{
					mKeyStrings += cpg::input::KeyCodeNames::get( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) );
					mKeyStrings += "\n";
				}
			}
			mKeyViewer->setString( mKeyStrings );
		}
	}
}
