#pragma once

#include "2d/CCNode.h"

namespace step_mole
{
	struct CircleCollisionComponentConfig;
}

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyNode : public cocos2d::Node
		{
		public:
			struct DebugConfig
			{
				bool bShowPivot = false;
			};

		private:
			EnemyNode();

		public:
			static EnemyNode* create(
				const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);

		private:
			bool init(
				const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);
		};
	}
}
