#pragma once

#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class SpawnProcessor_CircularSector_01_1Direction : public iSpawnProcessor
		{
		private:
			SpawnProcessor_CircularSector_01_1Direction(
				const StageConfig& stage_config
				, const bool rotate_direction_left
				, const float degree_per_cycle
				, const int bullets_per_cycle
				, const int repeat_count
				, const float total_time
			);

		public:
			static SpawnProcessorUp Create(
				const StageConfig& stage_config
				, const bool rotate_direction_left
				, const float degree_per_cycle
				, const int bullets_per_cycle
				, const int repeat_count
				, const float total_time
			);

			void init() override;
			void Enter( const cocos2d::Vec2& target_position ) override;
			bool Update( float dt, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			const float mHalfRadianPerCycle;
			const int mBulletsPerCycle;
			const int mRequiredBulletCount;
			const float mRadianPerBullet;
			const float mSecondsPerBullet;

			float mRemainTime;
			cocos2d::Vec2 mPivotPosition;
			cocos2d::Vec2 mFireStartDirection;
			int mCurrentFireCount;
		};
	}
}
