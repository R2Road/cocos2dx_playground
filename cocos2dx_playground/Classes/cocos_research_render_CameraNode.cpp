#include "cocos_research_render_CameraNode.h"

#include <new>

#include "base/CCDirector.h"

USING_NS_CC;

namespace cocos_research_render
{
	CameraNode::CameraNode() :
		mainlayer( nullptr )
		, mPivotNode( nullptr )

		, mCenter()
	{}

	CameraNode* CameraNode::create()
	{
		auto ret = new ( std::nothrow ) CameraNode();
		if( ret && ret->init() )
		{
			ret->scheduleUpdateWithPriority( 1 );
			ret->autorelease();
		}
		else
		{
			CC_SAFE_DELETE( ret );
		}

		return ret;
	}

	void CameraNode::update( float dt )
	{
		// # Summury
		// 1. get pivot world position
		// 2. make move vec : pivot 2 center
		// 3. get mainlayer world position
		// 4. make main layer new world position
		// 5. make main layer new local position

		// step : 1
		const auto pivot_world_position = mPivotNode->getParent()->convertToWorldSpace( mPivotNode->getPosition() );

		// step : 2
		const auto move_vec = mCenter - pivot_world_position;

		// step : 3
		const auto mainlayer_world_position = mainlayer->getParent()->convertToWorldSpace( mainlayer->getPosition() );

		// step : 4
		const auto mainlayer_new_world_position = mainlayer_world_position + move_vec;

		// step : 5
		const auto mainlayer_new_position = mainlayer->getParent()->convertToNodeSpace( mainlayer_new_world_position );

		// end
		mainlayer->setPosition( mainlayer_new_position );

		Node::update( dt );
	}
}