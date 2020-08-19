#pragma once

#include "step_rain_of_chaos_game_iEnemyProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Tie : public iEnemyProcessor
		{
		private:
			EnemyProcessor_Tie(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, EnemyProcessorUp&& processor_1
				, EnemyProcessorUp&& processor_2
			);

		public:

			static EnemyProcessorUp Create(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, EnemyProcessorUp&& processor_1
				, EnemyProcessorUp&& processor_2
			);

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			EnemyProcessorUp mProcessor_1;
			EnemyProcessorUp mProcessor_2;
		};
	}
}
