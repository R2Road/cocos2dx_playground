#pragma once

#include "math/Vec2.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Move_Orbit_01 : public iEnemyProcessor
		{
		private:
			EnemyProcessor_Move_Orbit_01(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, const float limit_time
				, const float distance_rate
			);

		public:

			static EnemyProcessorUp Create(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, const float limit_time
				, const float distance_rate
			);

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			const float mLimitTime;
			const float mDistance;

			cocos2d::Vec2 mStartPosition;
			cocos2d::Vec2 mMove;

			float mElapsedTime;
		};
	}
}
