#pragma once

#include "2d\CCNode.h"

namespace cocos_research_render
{
	class CameraNode : public cocos2d::Node
	{
	private:
		CameraNode();

	public:
		static CameraNode* create();

		void update( float dt ) override;

		void SetCenterWorldPosition( const cocos2d::Vec2 center ) { mCenter = center; }
		void SetMainLayer( cocos2d::Node* node ) { mainlayer = node; };
		void SetPivotNode( cocos2d::Node* pivot_node ) { mPivotNode = pivot_node; }

	private:
		cocos2d::Node* mainlayer;
		cocos2d::Node* mPivotNode;

		cocos2d::Vec2 mCenter;
	};
}

