#pragma once

#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN
	class Node;
NS_CC_END

namespace cpg_node
{
	class PivotNode
	{
	private:
		PivotNode() = delete;

	public:
		static cocos2d::Node* create();
	};
}
