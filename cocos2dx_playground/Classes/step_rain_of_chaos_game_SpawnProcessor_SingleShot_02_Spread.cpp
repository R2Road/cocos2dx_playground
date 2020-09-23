#include "step_rain_of_chaos_game_SpawnProcessor_SingleShot_02_Spread.h"

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
		SpawnProcessor_SingleShot_02_Spread::SpawnProcessor_SingleShot_02_Spread(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const bool rotate_direction_left
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float seconds_per_bullet
			, const float sleep_per_cycle
		) : iSpawnProcessor( stage_config, spawn_processor_config )
			, mRotateDirection( rotate_direction_left )
			, mBulletsPerCycle( std::max( 1, bullets_per_cycle ) )
			, mRequiredBulletCount( std::max( 1, mBulletsPerCycle * repeat_count ) )
			, mRadianPerBullet( CC_DEGREES_TO_RADIANS( ( degree_per_cycle * 0.5f ) / std::max( 1, mBulletsPerCycle - 1 ) ) )
			, mSecondsPerBullet( seconds_per_bullet )
			, mSleepPerCycle( sleep_per_cycle )

			, mStep( eStep::Fire )

			, mRemainTime( 0.f )
			, mStartPosition()
			, mTargetPosition()
			, mFireStartDirection()
			, mCurrentRotateDirection( false )
			, mCurrentFireCount( 0 )
			, mCurrentFireCountInCycle( 0 )

			, mElapsedTime4Sleep( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_SingleShot_02_Spread::Create(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const bool rotate_direction_left
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float seconds_per_bullet
			, const float sleep_per_cycle
		)
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_SingleShot_02_Spread(
				stage_config, spawn_processor_config
				, rotate_direction_left
				, degree_per_cycle, bullets_per_cycle
				, repeat_count
				, seconds_per_bullet
				, sleep_per_cycle
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_SingleShot_02_Spread::Enter( const Vec2& start_position, const Vec2& target_position )
		{
			mStep = eStep::Fire;

			mRemainTime = mSecondsPerBullet;

			mStartPosition = start_position;
			mTargetPosition = target_position;

			mFireStartDirection = mTargetPosition - mStartPosition;
			mFireStartDirection.normalize();

			mCurrentRotateDirection = mRotateDirection;
			mCurrentFireCount = 0;
			mCurrentFireCountInCycle = 0;
		}
		bool SpawnProcessor_SingleShot_02_Spread::Update( const float dt, const Vec2& start_position, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			if( eStep::Fire == mStep )
			{
				mRemainTime += dt;

				if( mSecondsPerBullet <= mRemainTime )
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
					}

					Vec2 temp_fire_direction;
					while( mSecondsPerBullet <= mRemainTime )
					{
						mRemainTime -= mSecondsPerBullet;

						temp_fire_direction = mFireStartDirection;
						temp_fire_direction.rotate( Vec2::ZERO, mRadianPerBullet * ( mCurrentRotateDirection ? 1 : -1 ) * mCurrentFireCountInCycle );

						out_spawn_info_container->push_back( SpawnInfo{
							mStartPosition
							, temp_fire_direction
						} );

						mCurrentRotateDirection = !mCurrentRotateDirection;
						++mCurrentFireCount;
						if( mRequiredBulletCount <= mCurrentFireCount )
						{
							break;
						}

						++mCurrentFireCountInCycle;
						if( mBulletsPerCycle <= mCurrentFireCountInCycle )
						{
							mCurrentFireCountInCycle = 0;

							mStep = eStep::Sleep;
							mElapsedTime4Sleep = 0.f;

							mRemainTime = mSecondsPerBullet;
							break;
						}
					}
				}
			}
			else
			{
				mElapsedTime4Sleep += dt;
				if( mSleepPerCycle <= mElapsedTime4Sleep )
				{
					mStep = eStep::Fire;
				}
			}

			return mRequiredBulletCount > mCurrentFireCount;
		}
	}
}
