#include "cpg_node_PivotNode.h"

#include "2d/CCDrawNode.h"

USING_NS_CC;

namespace cpg_node
{
	cocos2d::Node* PivotNode::create()
	{
		auto ret = DrawNode::create();
		ret->drawDot(
			Vec2::ZERO
			, 1.f
			, Color4F::GREEN
		);
		return ret;
	}
}