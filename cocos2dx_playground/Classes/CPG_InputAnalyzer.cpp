#include "CPG_InputAnalyzer.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		Analyzer::Analyzer() :
			key_map_container()
			, key_status_container()
		{}

		AnalyzerSp Analyzer::create()
		{
			AnalyzerSp ret( new ( std::nothrow ) Analyzer() );

			//
			// key status setup
			//
			{
				const std::vector<cocos2d::EventKeyboard::KeyCode> use_keys = {
					cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE
					, cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW
					, cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW
					, cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW
					, cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW
				};

				ret->key_map_container.reserve( use_keys.size() );
				int k_i = 0;
				for( const auto k : use_keys )
				{
					ret->key_map_container.emplace_back( KeyMapPiece{ k, k_i } );
					++k_i;
				}
			}

			ret->key_status_container.resize( ret->key_map_container.size() );


			return ret;
		}

		void Analyzer::onKeyPressed( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_map_container )
				if( keycode == k.keycode )
					key_status_container[k.idx] = true;
		}

		void Analyzer::onKeyReleased( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_map_container )
				if( keycode == k.keycode )
					key_status_container[k.idx] = false;
		}

		const bool Analyzer::getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const
		{
			for( auto& k : key_map_container )
				if( keycode == k.keycode )
					return key_status_container[k.idx];

			return false;
		}
	}
}