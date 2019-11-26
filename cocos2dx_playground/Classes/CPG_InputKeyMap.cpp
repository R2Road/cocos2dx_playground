#include "CPG_InputKeyMap.h"

#include <utility>

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		KeyMap::KeyMap( KeyMapContainer&& _container ) : container( std::move( _container ) ) {}

		KeyMapSp KeyMap::create()
		{
			const std::vector<cocos2d::EventKeyboard::KeyCode> use_keys = {
				cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE
				, cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW
				, cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW
				, cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW
				, cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW
			};

			KeyMapContainer container;
			container.reserve( use_keys.size() );

			int k_i = 0;
			for( const auto k : use_keys )
			{
				container.emplace_back( KeyMapPiece{ k, k_i } );
				++k_i;
			}

			KeyMapSp ret( new ( std::nothrow ) KeyMap( std::move( container ) ) );
			return ret;
		}
	}
}