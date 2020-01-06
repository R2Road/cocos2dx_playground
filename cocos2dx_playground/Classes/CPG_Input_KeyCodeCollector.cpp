#include "CPG_Input_KeyCodeCollector.h"

USING_NS_CC;

namespace cpg
{
	namespace input
	{
		KeyCodeCollector::KeyCodeCollector() : mContainer()
		{}

		void KeyCodeCollector::onKeyPressed( EventKeyboard::KeyCode keycode )
		{
			mContainer[static_cast<std::size_t>( keycode )] = true;
		}

		void KeyCodeCollector::onKeyReleased( EventKeyboard::KeyCode keycode )
		{
			mContainer[static_cast<std::size_t>( keycode )] = false;
		}
	}
}