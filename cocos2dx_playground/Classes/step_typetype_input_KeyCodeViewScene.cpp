#include "step_typetype_input_KeyCodeViewScene.h"

#include <sstream>

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_KeyCodeViewNode = 20140416;
}

namespace step_typetype
{
	namespace input
	{
		KeyCodeViewScene::KeyCodeViewScene() : mPressedKeyCount( 0 ), mKeyboardListener( nullptr ) {}

		Scene* KeyCodeViewScene::create()
		{
			auto ret = new ( std::nothrow ) KeyCodeViewScene();
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

		bool KeyCodeViewScene::init()
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
				ss << "+ Input - Key Code View";
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[Keyboard] : Show Key Code";

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
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// KeyCode View
			//
			{
				auto label = Label::createWithTTF( "Press Key", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_KeyCodeViewNode );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5, 0.5 ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, 9999 );

				clearKeyCodeView();
			}

			return true;
		}

		void KeyCodeViewScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( KeyCodeViewScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyCodeViewScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void KeyCodeViewScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Node::onExit();
		}

		void KeyCodeViewScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_typetype::RootScene::create() );
		}
		void KeyCodeViewScene::updateKeyCodeView( cocos2d::EventKeyboard::KeyCode keycode )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
			label->setString( std::to_string( static_cast<int>( keycode ) ) );
		}
		void KeyCodeViewScene::clearKeyCodeView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
			label->setString( "Press Key" );
		}

		void KeyCodeViewScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( KeyCodeViewScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( KeyCodeViewScene::updateForExit ), 0.f );
				}
			}
			else
			{
				++mPressedKeyCount;
				updateKeyCodeView( keycode );
			}
		}
		void KeyCodeViewScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				mPressedKeyCount = std::max( 0, mPressedKeyCount - 1 );
				if( 0 == mPressedKeyCount )
				{
					clearKeyCodeView();
				}
			}
		}
	}
}
