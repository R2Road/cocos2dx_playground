#pragma once

#include "CPG_Input_KeyCodeContainer.h"

namespace cpg
{
	namespace input
	{
		class KeyCodeCollector
		{
		public:
			KeyCodeCollector();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode );

			bool isActiveKey( const cocos2d::EventKeyboard::KeyCode keycode ) const { return mContainer[static_cast<std::size_t>( keycode )]; }

		private:
			KeyCodeContainerT mContainer;
		};
	}
}