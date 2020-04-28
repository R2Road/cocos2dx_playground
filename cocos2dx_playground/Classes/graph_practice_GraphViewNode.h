#pragma once

#include "2d/CCNode.h"

namespace graph_practice
{
	class GraphViewNode : public cocos2d::Node
	{
	private:
		GraphViewNode();

	public:
		static GraphViewNode* create( const char* title, const int width, const int height );

	private:
		bool init( const char* title, const int width, const int height );

	public:
		void UpdateView( const float g_x, const float g_y );

	private:
		cocos2d::Node* mViewNode;
	};
}
