#pragma once

#include <vector>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"

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

			using EnemyProcessorContainer = std::vector<EnemyProcessorUp>;

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

			void update4Processor( float delta_time );

		public:
			void SetProcessor( EnemyProcessorContainer&& enemy_processor_container );
			void StartProcess();

		private:
			EnemyProcessorContainer mProcessorContainer;
			EnemyProcessorContainer::iterator mCurrentProcessor;
		};
	}
}
