#pragma once

#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class SpawnProcessor_Circle_01_OutToIn : public iSpawnProcessor
		{
		private:
			SpawnProcessor_Circle_01_OutToIn(
				const StageConfig& stage_config
				, const SpawnProcessorConfig& spawn_processor_config
				, const bool rotate_direction_left
				, const int bullets_per_cycle
				, const float limit_time_per_cycle
				, const int repeat_count
			);

		public:
			static SpawnProcessorUp Create(
				const StageConfig& stage_config
				, const SpawnProcessorConfig& spawn_processor_config
				, const bool rotate_direction_left
				, const int bullets_per_cycle
				, const float limit_time_per_cycle
				, const int repeat_count
			);

			void Enter( const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position ) override;
			bool Update( const float dt, const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			const int mRequiredBulletCount;
			const float mRadianPerBullet;
			const float mSecondsPerBullet;

			float mRemainTime;
			cocos2d::Vec2 mCurrentStartPosition;
			int mCurrentFireCount;
		};
	}
}
