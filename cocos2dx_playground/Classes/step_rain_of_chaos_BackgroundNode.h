#pragma once

#include "2d/CCNode.h"

namespace step_rain_of_chaos
{
	class BackgroundNode : public cocos2d::Node
	{
	private:
		BackgroundNode();

	public:
		static BackgroundNode* create();

		bool init() override;
	};
}
