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

			inline const bool isActiveKey( const cocos2d::EventKeyboard::KeyCode _keycode ) const { return container[static_cast<std::size_t>( _keycode )]; }

		private:
			KeyCodeContainer container;
		};
	}
}