#pragma once

#include <vector>
#include <string>

#include "cocos/base/CCEventKeyboard.h"

namespace CPG
{
	namespace Input
	{
		class KeyMapConfigHelper
		{
		public:
			struct KeyMapAndNamePiece
			{
				KeyMapAndNamePiece( const std::string& _name, const int _idx, const cocos2d::EventKeyboard::KeyCode _keycode ) : name( _name ), idx( _idx ), keycode( _keycode ) {}

				const std::string name;
				const int idx;
				cocos2d::EventKeyboard::KeyCode keycode;
			};
			using KeyMapAndName_Container = std::vector<KeyMapAndNamePiece>;

		public:
			explicit KeyMapConfigHelper();

			const bool load( const char* _key_map_file_name );
			void save( const char* _key_map_file_name );
			inline const KeyMapAndName_Container& getContainer() const { return container; }
			void set( const int _key_index, const cocos2d::EventKeyboard::KeyCode _new_keycode );

		private:
			const bool load_Json( const char* _key_map_path );
			void save_Json( const char* _key_map_path ) const;

			KeyMapAndName_Container container;
		};
	}
}