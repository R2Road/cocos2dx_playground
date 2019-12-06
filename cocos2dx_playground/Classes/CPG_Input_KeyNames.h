#pragma once

#include "base/CCEventKeyboard.h"

namespace CPG
{
	namespace Input
	{
		class KeyNames
		{
		private:
			KeyNames() = delete;

		public:
			static const char* get( const cocos2d::EventKeyboard::KeyCode _keycode );
		};
	}
}