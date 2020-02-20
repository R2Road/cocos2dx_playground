#pragma once

#include <bitset>

#include "cocos2d.h"
#include "CPG_Input_KeyCodeContainer.h"

namespace cpg
{
	namespace input
	{
		class KeyCodeCollector
		{
		public:
			using ContainerT = KeyCodeContainerT;

			KeyCodeCollector();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

			inline const bool isActiveKey( const cocos2d::EventKeyboard::KeyCode keycode ) const { return mContainer[static_cast<std::size_t>( keycode )]; }

		private:
			ContainerT mContainer;
		};
	}
}