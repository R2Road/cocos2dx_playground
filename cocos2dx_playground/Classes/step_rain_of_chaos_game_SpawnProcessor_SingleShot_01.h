#pragma once

#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class SpawnProcessor_SingleShot_01 : public iSpawnProcessor
		{
		private:
			SpawnProcessor_SingleShot_01( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config, const int repeat_count, const float sleep_per_cycle );

		public:
			static SpawnProcessorUp Create( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config, const int repeat_count, const float sleep_per_cycle );

			void Enter( const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position ) override;
			bool Update( const float dt, const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			const int mRequiredCycle;
			const float mSleepPerCycle;

			int mStep;

			cocos2d::Vec2 mStartPosition;
			cocos2d::Vec2 mTargetPosition;
			cocos2d::Vec2 mFireStartDirection;
			int mCurrentFireCycle;

			float mElapsedTime4Sleep;
		};
	}
}
