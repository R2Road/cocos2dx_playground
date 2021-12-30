#pragma once

#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN
	class Node;
NS_CC_END

namespace cpg_node
{
	class GuideRectNode
	{
	private:
		GuideRectNode() = delete;

	public:
		static cocos2d::Node* create( cocos2d::Node* owner_node );
	};
}
