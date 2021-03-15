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
				KeyInfo( const char* name, const int idx, const cocos2d::EventKeyboard::KeyCode keycode, const char* sprite_frame_name ) :
					mName( name )
					, mIdx( idx )
					, mKeycode( keycode )
					, mSpriteFrameName( sprite_frame_name )
				{}

				const std::string mName;
				const int mIdx;
				cocos2d::EventKeyboard::KeyCode mKeycode;
				const std::string mSpriteFrameName;
			};
			using KeyMapAndName_Container = std::vector<KeyInfo>;

		public:
			explicit KeyMapConfigHelper();

			const bool load( const char* key_map_file_name );
			void save( const char* key_map_file_name );
			inline const KeyMapAndName_Container& getContainer() const { return mContainer; }
			void set( const int key_index, const cocos2d::EventKeyboard::KeyCode new_keycode );

		private:
			const bool load_Resource();
			const bool load_Json( const char* key_map_path );
			void save_Json( const char* key_map_path ) const;


		private:
			KeyMapAndName_Container mContainer;
		};
	}
}