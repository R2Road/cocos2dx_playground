#include "CPG_InputAnalyzer.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		Analyzer::Analyzer() : key_status_container() {}

		AnalyzerSp Analyzer::create()
		{
			AnalyzerSp ret( new ( std::nothrow ) Analyzer() );

			//
			// key status setup
			//
			ret->key_status_container.reserve( 5 );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW );
			ret->key_status_container.emplace_back( cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW );


			return ret;
		}

		void Analyzer::onKeyPressed( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_status_container )
				if( keycode == k.keycode )
					k.status = true;
		}

		void Analyzer::onKeyReleased( EventKeyboard::KeyCode keycode )
		{
			for( auto& k : key_status_container )
				if( keycode == k.keycode )
					k.status = false;
		}

		const bool Analyzer::getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const
		{
			for( auto& k : key_status_container )
				if( keycode == k.keycode )
					return k.status;

			return false;
		}
	}
}