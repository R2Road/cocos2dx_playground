#include "CPG_InputDelegator.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		Delegator::Delegator() : keyboard_listener( nullptr ), key_status_container() {}

		Delegator* Delegator::create()
		{
			auto ret = new ( std::nothrow ) Delegator();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}


			//
			// key status setup
			//
			ret->key_status_container.reserve( 5 );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW );


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
			for( auto& k : key_status_container )
				if( keycode == k.keycode )
					k.status = true;
		}

		void Delegator::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			for( auto& k : key_status_container )
				if( keycode == k.keycode )
					k.status = false;
		}

		const bool Delegator::getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const
		{
			for( auto& k : key_status_container )
				if( keycode == k.keycode )
					return k.status;

			return false;
		}
	}
}