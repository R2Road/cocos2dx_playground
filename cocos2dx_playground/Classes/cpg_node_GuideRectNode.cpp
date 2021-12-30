#include "cpg_node_GuideRectNode.h"

#include "2d/CCDrawNode.h"

USING_NS_CC;

namespace cpg_node
{
	cocos2d::Node* GuideRectNode::create( cocos2d::Node* owner_node )
	{
		auto ret = DrawNode::create();
		ret->setAnchorPoint( owner_node->getAnchorPoint() );
		ret->drawRect(
			Vec2::ZERO
			, Vec2( owner_node->getBoundingBox().size.width, owner_node->getBoundingBox().size.height )
			, Color4F::GREEN
		);
		return ret;
	}
}