#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace algorithm_practice_astar
{
	class CostNode;

	class CostMapNode: public cocos2d::Node
	{
	private:
		CostMapNode();

	public:
		static CostMapNode* create( const std::size_t map_width, const std::size_t map_height, const cocos2d::Size cost_node_size );

	private:
		bool init( const std::size_t map_width, const std::size_t map_height, const cocos2d::Size cost_node_size );

	private:
		std::vector<CostNode*> mContainer;
	};
}
