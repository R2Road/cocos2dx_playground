#pragma once

#include <memory>
#include <bitset>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		class AllowedKeys
		{
		public:
			static const std::size_t ContainerSize = static_cast<std::size_t>( static_cast<int>( cocos2d::EventKeyboard::KeyCode::KEY_PLAY ) + 1 );
			using Container = std::bitset<ContainerSize>;

			AllowedKeys();

		public:
			void load( const char* _allowed_keys_file_name );

		public:
			Container container;
		};
	}
}