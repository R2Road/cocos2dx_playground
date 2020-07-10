#pragma once

#include <functional>
#include <vector>

#include "2d/CCNode.h"
#include "math/CCGeometry.h"

#include "step_rain_of_chaos_game_StageConfig.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageNode : public cocos2d::Node
		{
		public:
			using TargetProcessExitCallback = std::function<void( int )>;
			struct DebugConfig
			{
				bool bShowPivot = false;
				bool bShowAreaGuide = false;
			};

		private:
			StageNode( const StageConfig stage_config );

		public:
			static StageNode* create( const StageConfig stage_config, const DebugConfig stage_node_config );

		private:
			bool init( const DebugConfig stage_node_config );

		private:
			const StageConfig mStageConfig;
		};
	}
}
