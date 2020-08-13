#pragma once

#include "math/Vec2.h"

#include "step_rain_of_chaos_game_iEnemyProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Move_Linear_01 : public iEnemyProcessor
		{
		private:
			EnemyProcessor_Move_Linear_01( const StageConfig& stage_config, cocos2d::Node* const owner_node );

		public:

			static EnemyProcessorUp Create( const StageConfig& stage_config, cocos2d::Node* const owner_node );

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			cocos2d::Vec2 mStartPosition;
			cocos2d::Vec2 mMove;

			float mLimitTime;
			float mElapsedTime;
		};
	}
}
