#pragma once

#include <vector>
#include <memory>

#include "cocos2d.h"

namespace cpg
{
	namespace input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;

		class KeyMap
		{
		public:
			struct KeyMapPiece
			{
				KeyMapPiece( const cocos2d::EventKeyboard::KeyCode keycode, const int idx ) : keycode( keycode ), idx( idx ) {}

				cocos2d::EventKeyboard::KeyCode keycode;
				int idx;
			};
			using KeyMapContainer = std::vector<KeyMapPiece>;

		private:
			explicit KeyMap( KeyMapContainer&& container );
			static KeyMapSp create_with_json( const char* key_map_path );

		public:
			static KeyMapSp create( const char* key_map_file_name );

			static const KeyMapSp& get_default();
			static const KeyMapSp& get_dummy();

			const int getKeyIndex( const cocos2d::EventKeyboard::KeyCode key_code ) const;

			const KeyMapContainer mContainer;
		};
	}
}