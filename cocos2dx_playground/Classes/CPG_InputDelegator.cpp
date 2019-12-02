#include "CPG_InputDelegator.h"

#include "CPG_Input_BasicCollector.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		Delegator::Delegator() : keyboard_listener( nullptr ) {}

		Delegator* Delegator::create()
		{
			auto ret = new ( std::nothrow ) Delegator();
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
			if( analyzer )
				analyzer->update();
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
			if( analyzer )
				analyzer->onKeyPressed( keycode );
		}

		void Delegator::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			if( analyzer )
				analyzer->onKeyReleased( keycode );
		}

		void Delegator::addAnalyzer( AnalyzerSp& _new_analyzer )
		{
			analyzer = _new_analyzer;
		}
	}
}