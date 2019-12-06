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
			using AllowedInfos = std::bitset<ContainerSize>;

			AllowedKeys();

		public:
			void load();

			inline const bool isAllowed( const cocos2d::EventKeyboard::KeyCode _target_key_code ) const { return container[static_cast<std::size_t>( _target_key_code )]; }

		private:
			AllowedInfos container;
		};
	}
}