#include "algorithm_practice_astar_CostMapNode.h"

#include <new>
#include <numeric>

#include "algorithm_practice_astar_CostNode.h"

#include "cpg_GridIndexConverter.h"
#include "cpg_node_PivotNode.h"
#include "cpg_node_GuideRectNode.h"

USING_NS_CC;

namespace algorithm_practice_astar
{
	CostMapNode::CostMapNode() : mContainer()
	{}

	CostMapNode* CostMapNode::create( const std::size_t map_width, const std::size_t map_height, const cocos2d::Size cost_node_size )
	{
		auto ret = new ( std::nothrow ) CostMapNode();
		if( !ret || !ret->init( map_width, map_height, cost_node_size ) )
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool CostMapNode::init( const std::size_t map_width, const std::size_t map_height, const cocos2d::Size cost_node_size )
	{
		if( !Node::init() )
		{
			return false;
		}

		setContentSize( Size( map_width * cost_node_size.width, map_height * cost_node_size.height ) );

		//
		// Debug
		//
		addChild( cpg_node::PivotNode::create(), std::numeric_limits<int>::max() );
		addChild( cpg_node::GuideRectNode::create( this ), std::numeric_limits<int>::max() );

		//
		// Map
		//
		{
			mContainer.resize( map_width * map_height );

			cpg::GridIndexConverter index_converter( map_width, map_height );

			for( int y = 0; map_width > y; ++y )
			{
				for( int x = 0; map_height > x; ++x )
				{
					auto cost_node = CostNode::create( cost_node_size );
					cost_node->setPosition( x * cost_node_size.width, y * cost_node_size.height );
					addChild( cost_node );

					mContainer[index_converter.To_Linear( x, y )] = cost_node;
				}
			}
		}

		return true;
	}
}
