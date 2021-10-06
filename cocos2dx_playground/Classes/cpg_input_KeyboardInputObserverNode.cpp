#include "cpg_input_KeyboardInputObserverNode.h"

#include <new>

#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"

USING_NS_CC;

namespace cpg_input
{
	KeyboardInputObserverNode::KeyboardInputObserverNode() :
		mKeyboardListener( nullptr )
		, mAllowedKeys()
		, mbFound( false )
	{}

	KeyboardInputObserverNode* KeyboardInputObserverNode::create( const char* allowed_keys_file_name )
	{
		auto ret = new ( std::nothrow ) KeyboardInputObserverNode();
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

	bool KeyboardInputObserverNode::init( const char* allowed_keys_file_name )
	{
		if( !Node::init() )
		{
			return false;
		}

		cpg_input::AllowedKeys::Load( mAllowedKeys, allowed_keys_file_name );

		scheduleUpdateWithPriority( 1 );

		return true;
	}

	void KeyboardInputObserverNode::onEnter()
	{
		Node::onEnter();

		CCASSERT( !mKeyboardListener, "" );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( KeyboardInputObserverNode::onKeyPressed, this );
		mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyboardInputObserverNode::onKeyReleased, this );
		getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
	}
	void KeyboardInputObserverNode::update( float dt )
	{
		mbFound = false;
		Node::update( dt );
	}
	void KeyboardInputObserverNode::onExit()
	{
		CCASSERT( mKeyboardListener, "" );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}

	void KeyboardInputObserverNode::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( mAllowedKeys[static_cast<std::size_t>( keycode )] )
		{
			mbFound = true;
		}
	}

	void KeyboardInputObserverNode::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( mAllowedKeys[static_cast<std::size_t>( keycode )] )
		{
			mbFound = true;
		}
	}
}