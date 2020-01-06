#pragma once

#include "base/CCEventKeyboard.h"

namespace cpg
{
	namespace input
	{
		class KeyCodeNames
		{
		private:
			KeyCodeNames() = delete;

		public:
			static const char* get( const cocos2d::EventKeyboard::KeyCode _keycode );
			static const char* get_longest();
		};
	}
}