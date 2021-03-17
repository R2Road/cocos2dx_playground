#pragma once

#include "base/CCEventKeyboard.h"

namespace cpg_input
{
	class KeyCodeNames
	{
	private:
		KeyCodeNames() = delete;

	public:
		static const char* Get( const cocos2d::EventKeyboard::KeyCode keycode );
		static const char* Get_Longest();
	};
}