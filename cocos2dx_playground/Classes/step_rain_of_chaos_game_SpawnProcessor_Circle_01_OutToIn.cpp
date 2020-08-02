#include "step_rain_of_chaos_game_SpawnProcessor_Circle_01_OutToIn.h"

#include <algorithm>

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		SpawnProcessor_Circle_01_OutToIn::SpawnProcessor_Circle_01_OutToIn(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const bool rotate_direction_left
			, const int bullets_per_cycle
			, const float limit_time_per_cycle
			, const int repeat_count
		) : iSpawnProcessor( stage_config, spawn_processor_config )
			, mRequiredBulletCount( bullets_per_cycle * repeat_count )
			, mRadianPerBullet( CC_DEGREES_TO_RADIANS( 360.f / bullets_per_cycle ) * ( rotate_direction_left ? 1 : -1 ) )
			, mSecondsPerBullet( limit_time_per_cycle / bullets_per_cycle )

			, mRemainTime( 0.f )
			, mCurrentStartPosition()
			, mCurrentFireCount( 0 )
		{}

		SpawnProcessorUp SpawnProcessor_Circle_01_OutToIn::Create(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const bool rotate_direction_left
			, const int bullets_per_cycle
			, const float limit_time_per_cycle
			, const int repeat_count
		)
		{
			CCASSERT( 0 < bullets_per_cycle, "" );
			CCASSERT( 0.f < limit_time_per_cycle, "" );
			CCASSERT( 0 < repeat_count, "" );

			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_Circle_01_OutToIn(
				stage_config
				, spawn_processor_config
				, rotate_direction_left
				, bullets_per_cycle
				, limit_time_per_cycle
				, repeat_count
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_Circle_01_OutToIn::Enter( const Vec2& start_position, const Vec2& /*target_position*/ )
		{
			mRemainTime = mSecondsPerBullet;

			mCurrentStartPosition = start_position;

			mCurrentFireCount = 0;
		}
		bool SpawnProcessor_Circle_01_OutToIn::Update( const float dt, const Vec2& /*start_position*/, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			mRemainTime += dt;

			while( mSecondsPerBullet <= mRemainTime )
			{
				mCurrentStartPosition.rotate( mStageConfig.GetCenter(), mRadianPerBullet );

				out_spawn_info_container->push_back( SpawnInfo{
					mCurrentStartPosition
					, target_position - mCurrentStartPosition
				} );

				++mCurrentFireCount;
				if( mRequiredBulletCount <= mCurrentFireCount )
				{
					break;
				}

				mRemainTime -= mSecondsPerBullet;
			}

			return mRequiredBulletCount > mCurrentFireCount;
		}
	}
}
