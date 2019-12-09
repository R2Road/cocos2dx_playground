#include "CPG_InputTest_KeyboardInputObserber.h"

#include <new>

USING_NS_CC;

namespace CPG
{
	namespace InputTest
	{
		KeyboardInputObserber::KeyboardInputObserber() : found( false ) {}

		KeyboardInputObserber* KeyboardInputObserber::create()
		{
			auto ret = new ( std::nothrow ) KeyboardInputObserber();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->scheduleUpdateWithPriority( 1 );

			ret->autorelease();
			return ret;
		}

		void KeyboardInputObserber::onEnter()
		{
			Node::onEnter();

			keyboard_listener = EventListenerKeyboard::create();
			keyboard_listener->onKeyPressed = CC_CALLBACK_2( KeyboardInputObserber::onKeyPressed, this );
			keyboard_listener->onKeyReleased = CC_CALLBACK_2( KeyboardInputObserber::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( keyboard_listener, 1 );
		}
		void KeyboardInputObserber::update( float _dt )
		{
			found = false;
			Node::update( _dt );
		}
		void KeyboardInputObserber::onExit()
		{
			if( keyboard_listener )
			{
				getEventDispatcher()->removeEventListener( keyboard_listener );
				keyboard_listener = nullptr;
			}
			Node::onExit();
		}

		void KeyboardInputObserber::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			found = true;
		}

		void KeyboardInputObserber::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			found = true;
		}
	}
}