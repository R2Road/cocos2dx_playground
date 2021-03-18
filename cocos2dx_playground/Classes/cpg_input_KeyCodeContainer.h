#pragma once

#include <bitset>

#include "base/CCEventKeyboard.h"

namespace cpg_input
{
	const std::size_t KeyCodeContainerFirst = static_cast<std::size_t>( cocos2d::EventKeyboard::KeyCode::KEY_NONE );
	const std::size_t KeyCodeContainerSize = static_cast<std::size_t>( static_cast<int>( cocos2d::EventKeyboard::KeyCode::KEY_PLAY ) + 1 );

	using KeyCodeContainerT = std::bitset<KeyCodeContainerSize>;
}