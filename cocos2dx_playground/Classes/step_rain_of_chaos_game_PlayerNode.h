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
		class PlayerNode : public cocos2d::Node
		{
		public:
			struct DebugConfig
			{
				bool bShowPivot = false;
			};

		private:
			PlayerNode();

		public:
			static PlayerNode* create(
				const float radius
				, const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);

		private:
			bool init(
				const float radius
				, const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);
		};
	}
}
