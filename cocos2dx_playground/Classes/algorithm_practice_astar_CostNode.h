#pragma once

#include "2d/CCNode.h"

namespace algorithm_practice_astar
{
	class CostNode: public cocos2d::Node
	{
	private:
		CostNode();

	public:
		static cocos2d::Node* create();

	private:
		bool init() override;
	};
}
