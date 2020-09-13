#pragma once

#include "math/Vec2.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"
#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Fire_Single : public iEnemyProcessor
		{
		private:
			EnemyProcessor_Fire_Single(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, SpawnProcessorUp&& spawn_processor
				, SpawnInfoContainer& spawn_info_container
			);

		public:

			static EnemyProcessorUp Create(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, SpawnProcessorUp&& spawn_processor
				, SpawnInfoContainer& spawn_info_container
			);

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			SpawnProcessorUp mSpawnProcessor;
			SpawnInfoContainer& mSpawnInfoContainer;
			bool mIsLive;
		};
	}
}
