#pragma once

#include <bitset>

#include "cocos2d.h"
#include "CPG_Input_AllowedKeys.h"

namespace CPG
{
	namespace Input
	{
		class KeyCodeCollector
		{
		public:
			using KeyCodeContainer = std::bitset<AllowedKeys::ContainerSize>;

			KeyCodeCollector();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

		private:
			KeyCodeContainer container;
		};
	}
}