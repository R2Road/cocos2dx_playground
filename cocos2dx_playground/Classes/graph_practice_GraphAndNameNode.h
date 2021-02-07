#pragma once

#include <functional>

#include "2d/CCNode.h"

namespace graph_practice
{
	class GraphViewNode;

	class GraphAndNameNode : public cocos2d::Node
	{
	public:
		struct Config
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		using EvaluatorFunc = std::function<float( float )>;

	private:
		GraphAndNameNode();

	public:
		static GraphAndNameNode* create( const Config config, const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func );

	private:
		bool init( const Config config, const char* title, const int part_width, const int part_height, const EvaluatorFunc& evaluator_func );

	public:
		//
		// g_x : zero to one
		//
		void UpdateView( const float g_x );

	private:
		GraphViewNode* mGraphViewNode;
	};
}
