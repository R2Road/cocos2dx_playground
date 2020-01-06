#include "CPG_InputTest_KeyboardInputObserver.h"

#include <new>

USING_NS_CC;

namespace cpg
{
	namespace InputTest
	{
		KeyboardInputObserver::KeyboardInputObserver() :
			found( false )
			, allowed_keys()
		{}

		KeyboardInputObserver* KeyboardInputObserver::create( const char* _allowed_keys_file_name )
		{
			auto ret = new ( std::nothrow ) KeyboardInputObserver();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->allowed_keys = input::AllowedKeys::load( _allowed_keys_file_name );

			ret->scheduleUpdateWithPriority( 1 );

			ret->autorelease();
			return ret;
		}

		void KeyboardInputObserver::onEnter()
		{
			Node::onEnter();

			keyboard_listener = EventListenerKeyboard::create();
			keyboard_listener->onKeyPressed = CC_CALLBACK_2( KeyboardInputObserver::onKeyPressed, this );
			keyboard_listener->onKeyReleased = CC_CALLBACK_2( KeyboardInputObserver::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( keyboard_listener, 1 );
		}
		void KeyboardInputObserver::update( float _dt )
		{
			found = false;
			Node::update( _dt );
		}
		void KeyboardInputObserver::onExit()
		{
			if( keyboard_listener )
			{
				getEventDispatcher()->removeEventListener( keyboard_listener );
				keyboard_listener = nullptr;
			}
			Node::onExit();
		}

		void KeyboardInputObserver::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			if( allowed_keys[static_cast<std::size_t>( keycode )] )
				found = true;
		}

		void KeyboardInputObserver::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			if( allowed_keys[static_cast<std::size_t>( keycode )] )
				found = true;
		}
	}
}