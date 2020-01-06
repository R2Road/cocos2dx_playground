#pragma once

#include <vector>
#include <memory>

#include "cocos2d.h"

namespace cpg
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;

		class KeyMap
		{
		public:
			struct KeyMapPiece
			{
				KeyMapPiece( const cocos2d::EventKeyboard::KeyCode _keycode, const int _idx ) : keycode( _keycode ), idx( _idx ) {}

				cocos2d::EventKeyboard::KeyCode keycode;
				int idx;
			};
			using KeyMapContainer = std::vector<KeyMapPiece>;

		private:
			explicit KeyMap( KeyMapContainer&& _container );
			static KeyMapSp create_with_json( const char* _key_map_path );

		public:
			static KeyMapSp create( const char* _key_map_file_name );

			static const KeyMapSp& get_default();
			static const KeyMapSp& get_dummy();

			const int getKeyIndex( const cocos2d::EventKeyboard::KeyCode _key_code ) const;

			const KeyMapContainer container;
		};
	}
}