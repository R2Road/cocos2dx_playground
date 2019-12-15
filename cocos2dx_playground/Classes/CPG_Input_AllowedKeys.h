#pragma once

#include <bitset>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		class AllowedKeys
		{
		public:
			static const std::size_t ContainerFirst = static_cast<std::size_t>( cocos2d::EventKeyboard::KeyCode::KEY_NONE );
			static const std::size_t ContainerSize = static_cast<std::size_t>( static_cast<int>( cocos2d::EventKeyboard::KeyCode::KEY_PLAY ) + 1 );
			using Container = std::bitset<ContainerSize>;

			AllowedKeys() = delete;

			static const Container load( const char* _allowed_keys_file_name );
		};
	}
}