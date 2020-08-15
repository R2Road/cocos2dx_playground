#pragma once

#include "math/Vec2.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Move_CircularSector_01 : public iEnemyProcessor
		{
		private:
			EnemyProcessor_Move_CircularSector_01(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, const float limit_time
				, const bool direction
				, const float degree_per_cycle
			);

		public:

			static EnemyProcessorUp Create(
				const StageConfig& stage_config
				, cocos2d::Node* const owner_node
				, cocos2d::Node* const target_node
				, const float limit_time
				, const bool direction
				, const float degree_per_cycle
			);

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			const float mLimitTime;
			const float mRadianPerCycle;

			cocos2d::Vec2 mMove;

			float mElapsedTime;
		};
	}
}
