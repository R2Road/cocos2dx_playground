#include "CPG_Input_BasicCollector.h"

#include "CPG_InputKeyMap.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		BasicCollector::BasicCollector( const KeyMapSp& _key_map_container ) :
			key_map_container( _key_map_container )
			, key_history()
			, current_key_status_container()
		{
			last_key_status_container = key_history.begin();
			current_key_status_container = last_key_status_container + 1;
		}

		BasicCollectorSp BasicCollector::create( const KeyMapSp& _key_map_container )
		{
			BasicCollectorSp ret( new ( std::nothrow ) BasicCollector( _key_map_container ) );

			return ret;
		}

		void BasicCollector::update()
		{
			if( last_key_status_container->to_ulong() != current_key_status_container->to_ulong() )
			{
				last_key_status_container = current_key_status_container;
				++current_key_status_container;
				if( key_history.end() == current_key_status_container )
					current_key_status_container = key_history.begin();
				*current_key_status_container = *last_key_status_container;
			}
		}

		void BasicCollector::onKeyPressed( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_map_container->container )
				if( keycode == k.keycode )
					( *current_key_status_container )[k.idx] = true;
		}

		void BasicCollector::onKeyReleased( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_map_container->container )
				if( keycode == k.keycode )
					( *current_key_status_container )[k.idx] = false;
		}

		const bool BasicCollector::getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const
		{
			for( auto& k : key_map_container->container )
				if( keycode == k.keycode )
					return ( *current_key_status_container )[k.idx];

			return false;
		}
		const bool BasicCollector::getKeyStatus( const int target_key_index ) const
		{
			if( 0 > target_key_index || static_cast<std::size_t>( target_key_index ) >= ( *current_key_status_container ).size() )
				return false;

			return ( *current_key_status_container )[target_key_index];
		}
		const bool BasicCollector::hasChanged() const
		{
			return last_key_status_container->to_ulong() != current_key_status_container->to_ulong();
		}
	}
}