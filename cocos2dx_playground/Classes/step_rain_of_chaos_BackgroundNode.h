#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace step_rain_of_chaos
{
	class BackgroundNode : public cocos2d::Node
	{
	public:
		using SpriteFrameContainerT = std::vector<cocos2d::SpriteFrame*>;

	private:
		BackgroundNode();

	public:
		static BackgroundNode* create(
			const std::size_t vertical_amount
			, const std::size_t horizontal_amount
			, const char* texture_path
			, const SpriteFrameContainerT& sprite_frame_container
		);

		bool init(
			const std::size_t vertical_amount
			, const std::size_t horizontal_amount
			, const char* texture_path
			, const SpriteFrameContainerT& sprite_frame_container
		);
	};
}
