#pragma once

#include <functional>

#include "math/Vec2.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"
#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Fire : public iEnemyProcessor
		{
		public:
			using RequestBulletCallback = std::function<void( cocos2d::Vec2,cocos2d::Vec2 )>;

		private:
			EnemyProcessor_Fire(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, const RequestBulletCallback& request_bullet_callback
				, SpawnProcessorContainer&& spawn_processor_container
			);

		public:

			static EnemyProcessorUp Create(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, const RequestBulletCallback& request_bullet_callback
				, SpawnProcessorContainer&& spawn_processor_container
			);

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			const RequestBulletCallback mRequestBulletCallback;
			SpawnProcessorContainer mSpawnProcessorContainer;
			SpawnProcessorContainer::iterator mCurrentSpawnProcessor;
			SpawnInfoContainer mSpawnInfoContainer;
		};
	}
}
