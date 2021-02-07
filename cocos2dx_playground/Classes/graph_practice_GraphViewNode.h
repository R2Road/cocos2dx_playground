#pragma once

#include <functional>

#include "2d/CCNode.h"

namespace graph_practice
{
	class GraphViewNode : public cocos2d::Node
	{
	public:
		struct Config
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		using EvaluatorFunc = std::function<float( float )>;

	private:
		GraphViewNode( const int part_width, const int part_height, const EvaluatorFunc& evaluator_func );

	public:
		static GraphViewNode* create( const Config config, const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func );

	private:
		bool init( const Config config, const char* title );

	public:
		//
		// g_x : zero to one
		//
		void UpdateView( const float g_x );

	private:
		const cocos2d::Size mPartSize;
		const EvaluatorFunc mEvaluatorFunc;

		cocos2d::Node* mIndicatorNode;
		cocos2d::Node* mIndicatorXNode;
		cocos2d::Node* mIndicatorYNode;
		cocos2d::Node* mIndicatorBridgeXNode;
		cocos2d::Node* mIndicatorBridgeYNode;
		cocos2d::Node* mIndicatorY2Node;
	};
}
