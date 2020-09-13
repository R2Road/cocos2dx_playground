#pragma once

#include "math/Vec2.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"
#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Fire_Chain : public iEnemyProcessor
		{
		private:
			EnemyProcessor_Fire_Chain(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, SpawnProcessorPackage&& spawn_processor_container
				, SpawnInfoContainer& spawn_info_container
			);

		public:

			static EnemyProcessorUp Create(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, SpawnProcessorPackage&& spawn_processor_container
				, SpawnInfoContainer& spawn_info_container
			);

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			SpawnProcessorPackage mSpawnProcessorContainer;
			SpawnProcessorPackage::iterator mCurrentSpawnProcessor;
			SpawnInfoContainer& mSpawnInfoContainer;
		};
	}
}
