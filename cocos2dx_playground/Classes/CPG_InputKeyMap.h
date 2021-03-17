#pragma once

#include <memory>
#include <vector>

#include "base/CCEventKeyboard.h"

namespace cpg_input
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

		int getKeyIndex( const cocos2d::EventKeyboard::KeyCode key_code ) const;

		const KeyMapContainer mContainer;
	};
}