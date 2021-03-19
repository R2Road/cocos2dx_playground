#include "cpg_input_KeyboardInputObserver.h"

#include <new>

#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"

USING_NS_CC;

namespace cpg_input
{
	KeyboardInputObserver::KeyboardInputObserver() :
		mKeyboardListener( nullptr )
		, mAllowedKeys()
		, mbFound( false )
	{}

	KeyboardInputObserver* KeyboardInputObserver::create( const char* allowed_keys_file_name )
	{
		auto ret = new ( std::nothrow ) KeyboardInputObserver();
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

	bool KeyboardInputObserver::init( const char* allowed_keys_file_name )
	{
		if( !Node::init() )
		{
			return false;
		}

		cpg_input::AllowedKeys::Load( mAllowedKeys, allowed_keys_file_name );

		scheduleUpdateWithPriority( 1 );

		return true;
	}

	void KeyboardInputObserver::onEnter()
	{
		Node::onEnter();

		CCASSERT( !mKeyboardListener, "" );
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
		CCASSERT( mKeyboardListener, "" );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}

	void KeyboardInputObserver::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( mAllowedKeys[static_cast<std::size_t>( keycode )] )
		{
			mbFound = true;
		}
	}

	void KeyboardInputObserver::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( mAllowedKeys[static_cast<std::size_t>( keycode )] )
		{
			mbFound = true;
		}
	}
}