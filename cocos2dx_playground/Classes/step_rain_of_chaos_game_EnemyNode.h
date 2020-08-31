#pragma once

#include <vector>

#include "2d/CCNode.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"
#include "step_rain_of_chaos_game_iSpawnProcessor.h"

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

			using RequestBulletCallback = std::function<void( cocos2d::Vec2, cocos2d::Vec2 )>;

		private:
			EnemyNode( const RequestBulletCallback& request_bullet_callback );

		public:
			static EnemyNode* create(
				const float radius
				, const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
				, const RequestBulletCallback& request_bullet_callback
			);

		private:
			bool init(
				const float radius
				, const DebugConfig debug_config
				, const step_mole::CircleCollisionComponentConfig& circle_collision_component_config
			);

			void update4Processor( float delta_time );

		public:
			void StartProcess( EnemyProcessorContainer* enemy_processor_container );
			void StopProcess();

			SpawnInfoContainer& GetSpawnInfoContainer() { return mSpawnInfoContainer; }

		private:
			const RequestBulletCallback mRequestBulletCallback;

			EnemyProcessorContainer* mProcessorContainer;
			EnemyProcessorContainer::iterator mCurrentProcessor;
			SpawnInfoContainer mSpawnInfoContainer;
		};
	}
}
