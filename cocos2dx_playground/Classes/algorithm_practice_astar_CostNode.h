#pragma once

#include "2d/CCNode.h"

NS_CC_BEGIN
	class DrawNode;
	class Label;
NS_CC_END

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

	public:
		void SetCost( const int cost_2_start, const int cost_2_end );
		void SetStatus( const bool bOpen );

	private:
		cocos2d::DrawNode* mStatusNode;
		cocos2d::Label* mLabel_Cost2Start;
		cocos2d::Label* mLabel_Cost2End;
		cocos2d::Label* mLabel_TotalCost;
	};
}
