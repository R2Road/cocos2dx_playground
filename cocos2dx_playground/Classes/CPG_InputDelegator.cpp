#include "CPG_InputDelegator.h"

#include "CPG_Input_BasicCollector.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		Delegator::Delegator() :
			keyboard_listener( nullptr )
			, allowed_keys()
			, keycode_collector()
			, input_collector( nullptr )
		{}

		Delegator* Delegator::create( const char* _allowed_keys_file_name )
		{
			auto ret = new ( std::nothrow ) Delegator();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->allowed_keys.load( _allowed_keys_file_name );

			ret->scheduleUpdateWithPriority( 1 );
			ret->autorelease();
			return ret;
		}

		void Delegator::onEnter()
		{
			Node::onEnter();

			keyboard_listener = EventListenerKeyboard::create();
			keyboard_listener->onKeyPressed = CC_CALLBACK_2( Delegator::onKeyPressed, this );
			keyboard_listener->onKeyReleased = CC_CALLBACK_2( Delegator::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( keyboard_listener, 1 );
		}
		void Delegator::update( float _dt )
		{
			if( input_collector )
				input_collector->update();
			Node::update( _dt );
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
			if( !allowed_keys.isAllowed( keycode ) )
				return;

			keycode_collector.onKeyPressed( keycode );

			if( input_collector )
				input_collector->onKeyPressed( keycode );
		}

		void Delegator::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			if( !allowed_keys.isAllowed( keycode ) )
				return;

			keycode_collector.onKeyReleased( keycode );

			if( input_collector )
				input_collector->onKeyReleased( keycode );
		}

		void Delegator::addInputCollector( BasicCollectorSp& _new_input_collector )
		{
			input_collector = _new_input_collector;
		}
	}
}