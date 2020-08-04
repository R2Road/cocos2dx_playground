#pragma once

#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class SpawnProcessor_CircularSector_01_2Direction : public iSpawnProcessor
		{
		private:
			SpawnProcessor_CircularSector_01_2Direction(
				const StageConfig& stage_config
				, const SpawnProcessorConfig& spawn_processor_config
				, const bool rotate_direction_left
				, const float degree_per_cycle
				, const int bullets_per_cycle
				, const int repeat_count
				, const float seconds_per_bullet
				, const float delay_per_cycle
			);

		public:
			static SpawnProcessorUp Create(
				const StageConfig& stage_config
				, const SpawnProcessorConfig& spawn_processor_config
				, const bool rotate_direction_left
				, const float degree_per_cycle
				, const int bullets_per_cycle
				, const int repeat_count
				, const float seconds_per_bullet
				, const float sleep_per_cycle
			);

			void Enter( const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position ) override;
			bool Update( const float dt, const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			const float mHalfRadianPerCycle;
			const int mBulletsPerCycle;
			const int mRequiredBulletCount;
			const float mRadianPerBullet;
			const float mSecondsPerBullet;
			const float mSleepPerCycle;

			int mStep;

			float mRemainTime;
			cocos2d::Vec2 mStartPosition;
			cocos2d::Vec2 mTargetPosition;
			cocos2d::Vec2 mPivotDirection;
			cocos2d::Vec2 mCurrentFireStartDirection;
			float mCurrentRadianPerBullet;
			int mCurrentFireCount;
			int mCurrentFireCountInCycle;
			
			float mElapsedTime4Sleep;
		};
	}
}
