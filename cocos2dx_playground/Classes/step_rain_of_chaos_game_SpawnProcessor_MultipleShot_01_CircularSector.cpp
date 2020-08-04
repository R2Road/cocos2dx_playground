#include "step_rain_of_chaos_game_SpawnProcessor_MultipleShot_01_CircularSector.h"

#include <algorithm>

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace
{
	enum eStep
	{
		Fire,
		Sleep
	};
}

namespace step_rain_of_chaos
{
	namespace game
	{
		SpawnProcessor_MultipleShot_01_CircularSector::SpawnProcessor_MultipleShot_01_CircularSector(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float sleep_per_cycle
		) : iSpawnProcessor( stage_config, spawn_processor_config )
			, mHalfRadianPerCycle( CC_DEGREES_TO_RADIANS( degree_per_cycle * 0.5f ) )
			, mBulletsPerCycle( std::max( 2, bullets_per_cycle ) )
			, mRequiredCycle( std::max( 1, repeat_count ) )
			, mRadianPerBullet( CC_DEGREES_TO_RADIANS( degree_per_cycle / std::max( 1, mBulletsPerCycle - 1 ) ) )
			, mSleepPerCycle( sleep_per_cycle )

			, mStep( eStep::Fire )

			, mStartPosition()
			, mTargetPosition()
			, mFireStartDirection()
			, mCurrentFireCycle( 0 )

			, mElapsedTime4Sleep( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_MultipleShot_01_CircularSector::Create(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float sleep_per_cycle
		)
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_MultipleShot_01_CircularSector(
				stage_config
				, spawn_processor_config
				, degree_per_cycle, bullets_per_cycle
				, repeat_count
				, sleep_per_cycle
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_MultipleShot_01_CircularSector::Enter( const Vec2& start_position, const Vec2& target_position )
		{
			mStep = eStep::Fire;

			mStartPosition = start_position;
			mTargetPosition = target_position;

			mFireStartDirection = mTargetPosition - mStartPosition;
			mFireStartDirection.normalize();
			mFireStartDirection.rotate( Vec2::ZERO, -mHalfRadianPerCycle );

			mCurrentFireCycle = 0;
		}
		bool SpawnProcessor_MultipleShot_01_CircularSector::Update( const float dt, const Vec2& start_position, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			if( eStep::Fire == mStep )
			{
				if( mSpawnProcessorConfig.UpdateStartPosition )
				{
					mStartPosition = start_position;
				}

				if( mSpawnProcessorConfig.UpdateTargetPosition )
				{
					mTargetPosition = target_position;
				}

				if( mSpawnProcessorConfig.UpdateStartPosition || mSpawnProcessorConfig.UpdateTargetPosition )
				{
					mFireStartDirection = mTargetPosition - mStartPosition;
					mFireStartDirection.normalize();
					mFireStartDirection.rotate( Vec2::ZERO, -mHalfRadianPerCycle );
				}

				Vec2 temp_fire_direction;
				for( int i = 0; mBulletsPerCycle > i; ++i )
				{
					temp_fire_direction = mFireStartDirection;
					temp_fire_direction.rotate( Vec2::ZERO, mRadianPerBullet * i );

					out_spawn_info_container->push_back( SpawnInfo{
						mStartPosition
						, temp_fire_direction
					} );
				}

				++mCurrentFireCycle;

				mStep = eStep::Sleep;
				mElapsedTime4Sleep = 0.f;
			}
			else
			{
				mElapsedTime4Sleep += dt;
				if( mSleepPerCycle <= mElapsedTime4Sleep )
				{
					mStep = eStep::Fire;
				}
			}

			return mRequiredCycle > mCurrentFireCycle;
		}
	}
}
