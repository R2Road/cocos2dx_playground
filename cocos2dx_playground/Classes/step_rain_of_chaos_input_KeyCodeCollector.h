#pragma once

#include "cpg_input_KeyCodeContainer.h"

namespace step_rain_of_chaos
{
	namespace input
	{
		class KeyCodeCollector
		{
		public:
			using ContainerT = cpg_input::KeyCodeContainerT;

			KeyCodeCollector();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

			bool isActiveKey( const cocos2d::EventKeyboard::KeyCode keycode ) const { return mContainer[static_cast<std::size_t>( keycode )]; }

		private:
			ContainerT mContainer;
		};
	}
}