#pragma once

#include "2d/CCNode.h"

namespace algorithm_practice_astar
{
	class CostNode: public cocos2d::Node
	{
	private:
		CostNode();

	public:
		static CostNode* create( const cocos2d::Size node_size );

	private:
		bool init( const cocos2d::Size node_size );
	};
}
