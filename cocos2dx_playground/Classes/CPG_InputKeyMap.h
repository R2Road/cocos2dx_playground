#pragma once

#include <vector>
#include <memory>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;

		class KeyMap
		{
		private:
			struct KeyMapPiece
			{
				KeyMapPiece( const cocos2d::EventKeyboard::KeyCode _keycode, const int _idx ) : keycode( _keycode ), idx( _idx ) {}

				cocos2d::EventKeyboard::KeyCode keycode;
				int idx;
			};
			using KeyMapContainer = std::vector<KeyMapPiece>;

			explicit KeyMap( KeyMapContainer&& _container );

		public:
			static KeyMapSp create( const char* _key_map_path );
			static const KeyMapSp& get_dummy();

			const int getKeyIndex( const cocos2d::EventKeyboard::KeyCode _key_code ) const;

			const KeyMapContainer container;
		};
	}
}