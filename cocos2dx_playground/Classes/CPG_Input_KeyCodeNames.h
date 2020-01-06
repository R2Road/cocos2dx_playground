#pragma once

#include "base/CCEventKeyboard.h"

namespace cpg
{
	namespace Input
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