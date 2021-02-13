#pragma once

#include <functional>

#include "2d/CCNode.h"

namespace graph_practice
{
	class GraphViewNode : public cocos2d::Node
	{
	public:
		struct DebugConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		struct Config
		{
			int PartWidth = 50;
			int PartHeight = 50;
			float AllowedTimeX = 1.f;
			float AllowedTimeY = 1.f;
		};

		using EvaluatorFunc = std::function<float( float )>;

	private:
		GraphViewNode( const Config config, const EvaluatorFunc& evaluator_func );

	public:
		static GraphViewNode* create( const DebugConfig debug_config, const Config config, const EvaluatorFunc& evaluator_func );

	private:
		bool init( const DebugConfig debug_config );

	public:
		void UpdateView( const float g_x );

	private:
		const Config mConfig;
		const EvaluatorFunc mEvaluatorFunc;

		cocos2d::Node* mIndicatorNode;
		cocos2d::Node* mIndicatorXNode;
		cocos2d::Node* mIndicatorYNode;
		cocos2d::Node* mIndicatorBridgeXNode;
		cocos2d::Node* mIndicatorBridgeYNode;
		cocos2d::Node* mIndicatorY2Node;
	};
}
