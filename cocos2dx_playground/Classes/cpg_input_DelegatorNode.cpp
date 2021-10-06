#include "cpg_input_DelegatorNode.h"

#include <new>

#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"

#include "cpg_input_BasicKeyCollector.h"

USING_NS_CC;

namespace cpg_input
{
	DelegatorNode::DelegatorNode() :
		mKeyboardListener( nullptr )
		, mAllowedKeys()
		, mKeycodeCollector()
		, mInputCollector( nullptr )
	{}

	DelegatorNode* DelegatorNode::create( const char* allowed_keys_file_name )
	{
		auto ret = new ( std::nothrow ) DelegatorNode();
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

	bool DelegatorNode::init( const char* allowed_keys_file_name )
	{
		if( !Node::init() )
		{
			return false;
		}

		AllowedKeys::Load( mAllowedKeys, allowed_keys_file_name );

		scheduleUpdateWithPriority( -1 );

		return true;
	}

	void DelegatorNode::onEnter()
	{
		Node::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( DelegatorNode::onKeyPressed, this );
		mKeyboardListener->onKeyReleased = CC_CALLBACK_2( DelegatorNode::onKeyReleased, this );
		getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
	}
	void DelegatorNode::update( float dt )
	{
		if( mInputCollector )
		{
			mInputCollector->update_forHistory();

			mInputCollector->collect( mKeycodeCollector );
		}

		Node::update( dt );
	}
	void DelegatorNode::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Node::onExit();
	}

	void DelegatorNode::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( !mAllowedKeys[static_cast<std::size_t>( keycode )] )
		{
			return;
		}

		mKeycodeCollector.onKeyPressed( keycode );
	}

	void DelegatorNode::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( !mAllowedKeys[static_cast<std::size_t>( keycode )] )
		{
			return;
		}

		mKeycodeCollector.onKeyReleased( keycode );
	}

	void DelegatorNode::addInputCollector( KeyCollectorSp& new_key_collector )
	{
		mInputCollector = new_key_collector;
	}
}