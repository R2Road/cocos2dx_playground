#pragma once

#include <vector>

#include "2d/CCNode.h"

#include "cpg_GridIndexConverter.h"

namespace algorithm_practice_astar
{
	class CostNode;

	class CostMapNode: public cocos2d::Node
	{
	private:
		CostMapNode( const std::size_t map_width, const std::size_t map_height );

	public:
		static CostMapNode* create( const std::size_t map_width, const std::size_t map_height, const cocos2d::Size cost_node_size );

	private:
		bool init( const std::size_t map_width, const std::size_t map_height, const cocos2d::Size cost_node_size );

	public:
		void Open( const int target_x, const int target_y, const int cost_2_start, const int cost_2_end );
		void Close( const int target_x, const int target_y );
		void Reset();

	private:
		const cpg::GridIndexConverter mIndexConverter;
		std::vector<CostNode*> mContainer;
	};
}
