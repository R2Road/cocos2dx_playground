#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class BackgroundNode : public cocos2d::Node
		{
		public:
			using SpriteFrameContainerT = std::vector<cocos2d::SpriteFrame*>;

		private:
			BackgroundNode( SpriteFrameContainerT&& sprite_frame_container );

		public:
			static BackgroundNode* create(
				const std::size_t horizontal_reserve_amount
				, const std::size_t vertical_reserve_amount
				, const char* texture_path
				, SpriteFrameContainerT&& sprite_frame_container
			);

		private:
			bool init(
				const std::size_t horizontal_reserve_amount
				, const std::size_t vertical_reserve_amount
				, const char* texture_path
			);

		public:
			void Reset( const std::size_t horizontal_amount, const std::size_t vertical_amount );

		private:
			const SpriteFrameContainerT mSpriteFrameContainer;
		};
	}
}
