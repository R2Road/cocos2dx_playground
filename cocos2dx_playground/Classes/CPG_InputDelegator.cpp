#include "CPG_InputDelegator.h"

#include <new>

#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"

#include "CPG_Input_BasicCollector.h"

USING_NS_CC;

namespace cpg
{
	namespace input
	{
		Delegator::Delegator() :
			mKeyboardListener( nullptr )
			, mAllowedKeys()
			, mKeycodeCollector()
			, mInputCollector( nullptr )
		{}

		Delegator* Delegator::create( const char* allowed_keys_file_name )
		{
			auto ret = new ( std::nothrow ) Delegator();
			if( !ret || !ret->init( allowed_keys_file_name ) )
			{
				delete ret;
				ret = nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool Delegator::init( const char* allowed_keys_file_name )
		{
			if( !Node::init() )
			{
				return false;
			}

			mAllowedKeys = AllowedKeys::load( allowed_keys_file_name );

			scheduleUpdateWithPriority( -1 );
			schedule( schedule_selector( Delegator::post_update ) );

			return true;
		}

		void Delegator::onEnter()
		{
			Node::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( Delegator::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( Delegator::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void Delegator::update( float dt )
		{
			if( mInputCollector )
				mInputCollector->collect( mKeycodeCollector );

			Node::update( dt );
		}
		void Delegator::post_update( float /*dt*/ )
		{
			if( mInputCollector )
				mInputCollector->update_forHistory();
		}
		void Delegator::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void Delegator::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( !mAllowedKeys[static_cast<std::size_t>( keycode )] )
			{
				return;
			}

			mKeycodeCollector.onKeyPressed( keycode );
		}

		void Delegator::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( !mAllowedKeys[static_cast<std::size_t>( keycode )] )
			{
				return;
			}

			mKeycodeCollector.onKeyReleased( keycode );
		}

		void Delegator::addInputCollector( KeyCollectorSp& new_key_collector )
		{
			mInputCollector = new_key_collector;
		}
	}
}