#pragma once

#include "2d/CCNode.h"

namespace algorithm_practice_floodfill
{
	class ProcessorNode : public cocos2d::Node
	{
	public:
		struct Config
		{
			int MapWidth = 10;
			int MapHeight = 10;
		};

	private:
		ProcessorNode( const Config config );

	public:
		static ProcessorNode* create( const Config config );

	private:
		bool init() override;

	private:
		const Config mConfig;
	};
}
