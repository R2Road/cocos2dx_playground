#pragma once

#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class SpawnProcessor_MultipleShot_01_CircularSector : public iSpawnProcessor
		{
		private:
			SpawnProcessor_MultipleShot_01_CircularSector(
				const StageConfig& stage_config
				, const SpawnProcessorConfig& spawn_processor_config
				, const float degree_per_cycle
				, const int bullets_per_cycle
				, const int repeat_count
				, const float sleep_per_cycle
			);

		public:
			static SpawnProcessorUp Create(
				const StageConfig& stage_config
				, const SpawnProcessorConfig& spawn_processor_config
				, const float degree_per_cycle
				, const int bullets_per_cycle
				, const int repeat_count
				, const float sleep_per_cycle
			);

			void Enter( const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position ) override;
			bool Update( const float dt, const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			const float mHalfRadianPerCycle;
			const int mBulletsPerCycle;
			const int mRequiredCycle;
			const float mRadianPerBullet;
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
