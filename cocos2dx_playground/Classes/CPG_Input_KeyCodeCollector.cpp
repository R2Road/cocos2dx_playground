#include "CPG_Input_KeyCodeCollector.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		KeyCodeCollector::KeyCodeCollector() : changed( false ), container()
		{}

		void KeyCodeCollector::onKeyPressed( EventKeyboard::KeyCode keycode )
		{
			changed = true;
			container[static_cast<std::size_t>( keycode )] = true;
		}

		void KeyCodeCollector::onKeyReleased( EventKeyboard::KeyCode keycode )
		{
			changed = true;
			container[static_cast<std::size_t>( keycode )] = false;
		}
	}
}