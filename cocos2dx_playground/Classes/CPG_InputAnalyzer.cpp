#include "CPG_InputAnalyzer.h"

#include "CPG_InputKeyMap.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		Analyzer::Analyzer( const KeyMapSp& _key_map_container ) :
			key_map_container( _key_map_container )
			, key_status_container()
		{}

		AnalyzerSp Analyzer::create( const KeyMapSp& _key_map_container )
		{
			AnalyzerSp ret( new ( std::nothrow ) Analyzer( _key_map_container ) );

			return ret;
		}

		void Analyzer::onKeyPressed( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_map_container->container )
				if( keycode == k.keycode )
					key_status_container[k.idx] = true;
		}

		void Analyzer::onKeyReleased( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_map_container->container )
				if( keycode == k.keycode )
					key_status_container[k.idx] = false;
		}

		const bool Analyzer::getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const
		{
			for( auto& k : key_map_container->container )
				if( keycode == k.keycode )
					return key_status_container[k.idx];

			return false;
		}
		const bool Analyzer::getKeyStatus( const int target_key_index ) const
		{
			if( 0 > target_key_index || static_cast<std::size_t>( target_key_index ) >= key_status_container.size() )
				return false;

			return key_status_container[target_key_index];
		}
	}
}