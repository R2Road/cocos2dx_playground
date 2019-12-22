#pragma once

#include "base/CCEventKeyboard.h"

namespace CPG
{
	namespace Input
	{
		class KeyCodeNames
		{
		private:
			KeyCodeNames() = delete;

		public:
			static const char* get( const cocos2d::EventKeyboard::KeyCode _keycode );
		};
	}
}