#pragma once

#include <vector>
#include <string>

#include "cocos/base/CCEventKeyboard.h"

namespace cpg
{
	namespace input_test
	{
		class KeyMapConfigHelper
		{
		public:
			struct KeyInfo
			{
				KeyInfo( const char* name, const int index, const cocos2d::EventKeyboard::KeyCode keycode, const char* sprite_frame_name ) :
					Name( name )
					, Index( index )
					, CocosKeyCode( keycode )
					, SpriteFrameName( sprite_frame_name )
				{}

				const std::string Name;
				const int Index;
				cocos2d::EventKeyboard::KeyCode CocosKeyCode;
				const std::string SpriteFrameName;
			};
			using ContainerT = std::vector<KeyInfo>;

		public:
			explicit KeyMapConfigHelper();

			bool Load( const char* key_map_file_name );
			void Save( const char* key_map_file_name );
			inline const ContainerT& GetContainer() const { return mContainer; }
			void Set( const int key_index, const cocos2d::EventKeyboard::KeyCode new_keycode );

		private:
			bool load_Resource();
			bool load_Json( const char* key_map_path );
			void save_Json( const char* key_map_path ) const;


		private:
			ContainerT mContainer;
		};
	}
}