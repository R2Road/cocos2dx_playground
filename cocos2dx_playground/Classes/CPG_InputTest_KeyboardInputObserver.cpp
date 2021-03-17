#include "CPG_InputTest_KeyboardInputObserver.h"

#include <new>

#include "cocos2d.h"

USING_NS_CC;

namespace cpg
{
	namespace input_test
	{
		KeyboardInputObserver::KeyboardInputObserver() :
			mbFound( false )
			, mAllowedKeys()
		{}

		KeyboardInputObserver* KeyboardInputObserver::create( const char* allowed_keys_file_name )
		{
			auto ret = new ( std::nothrow ) KeyboardInputObserver();
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

			ret->mAllowedKeys = cpg_input::AllowedKeys::load( allowed_keys_file_name );

			ret->scheduleUpdateWithPriority( 1 );
			return ret;
		}

		void KeyboardInputObserver::onEnter()
		{
			Node::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( KeyboardInputObserver::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyboardInputObserver::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void KeyboardInputObserver::update( float dt )
		{
			mbFound = false;
			Node::update( dt );
		}
		void KeyboardInputObserver::onExit()
		{
			if( mKeyboardListener )
			{
				getEventDispatcher()->removeEventListener( mKeyboardListener );
				mKeyboardListener = nullptr;
			}
			Node::onExit();
		}

		void KeyboardInputObserver::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( mAllowedKeys[static_cast<std::size_t>( keycode )] )
				mbFound = true;
		}

		void KeyboardInputObserver::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( mAllowedKeys[static_cast<std::size_t>( keycode )] )
				mbFound = true;
		}
	}
}