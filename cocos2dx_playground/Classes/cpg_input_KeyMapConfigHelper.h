#pragma once

#include <vector>
#include <string>

#include "base/CCEventKeyboard.h"

namespace cpg_input
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

		inline const ContainerT& GetContainer() const { return mContainer; }

		bool Load( const char* file_name );
		void Save( const char* file_name );
		void Set( const int key_index, const cocos2d::EventKeyboard::KeyCode new_keycode );

	private:
		bool load_Resource();
		bool load_Json( const char* path );
		void save_Json( const char* path ) const;	

	private:
		ContainerT mContainer;
	};
}