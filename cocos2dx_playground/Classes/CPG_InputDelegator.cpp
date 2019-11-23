#include "CPG_InputDelegator.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		Delegator::Delegator() : keyboard_listener( nullptr ), key_pressed_esc( false ) {}

		Delegator* Delegator::create()
		{
			auto ret = new ( std::nothrow ) Delegator();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->autorelease();

			return ret;
		}

		void Delegator::onEnter()
		{
			Node::onEnter();

			keyboard_listener = EventListenerKeyboard::create();
			keyboard_listener->onKeyPressed = CC_CALLBACK_2( Delegator::onKeyPressed, this );
			keyboard_listener->onKeyReleased = CC_CALLBACK_2( Delegator::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( keyboard_listener, 1 );
		}
		void Delegator::onExit()
		{
			if( keyboard_listener )
			{
				getEventDispatcher()->removeEventListener( keyboard_listener );
				keyboard_listener = nullptr;
			}
			Node::onExit();
		}

		void Delegator::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
			{
				key_pressed_esc = true;
			}
			break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void Delegator::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
			{
				key_pressed_esc = false;
			}
			break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}