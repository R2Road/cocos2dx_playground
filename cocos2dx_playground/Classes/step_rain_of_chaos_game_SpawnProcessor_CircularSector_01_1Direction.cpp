#include "step_rain_of_chaos_game_SpawnProcessor_CircularSector_01_1Direction.h"

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
		SpawnProcessor_CircularSector_01_1Direction::SpawnProcessor_CircularSector_01_1Direction(
			const StageConfig& stage_config
			, const bool rotate_direction_left
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float seconds_per_bullet
			, const float sleep_per_cycle
		) : iSpawnProcessor( stage_config )
			, mHalfRadianPerCycle( CC_DEGREES_TO_RADIANS( degree_per_cycle * 0.5f ) * ( rotate_direction_left ? 1 : -1 ) )
			, mBulletsPerCycle( std::max( 1, bullets_per_cycle ) )
			, mRequiredBulletCount( std::max( 1, mBulletsPerCycle * repeat_count ) )
			, mRadianPerBullet( CC_DEGREES_TO_RADIANS( degree_per_cycle / std::max( 1, mBulletsPerCycle - 1 ) ) * ( rotate_direction_left ? 1 : -1 ) )
			, mSecondsPerBullet( seconds_per_bullet )
			, mSleepPerCycle( sleep_per_cycle )

			, mStep( eStep::Fire )

			, mRemainTime( 0.f )
			, mPivotPosition( Vec2::UNIT_Y )
			, mFireStartDirection()
			, mCurrentFireCount( 0 )

			, mElapsedTime4Sleep( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_CircularSector_01_1Direction::Create(
			const StageConfig& stage_config
			, const bool rotate_direction_left
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float seconds_per_bullet
			, const float sleep_per_cycle
		)
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_CircularSector_01_1Direction(
				stage_config
				, rotate_direction_left
				, degree_per_cycle, bullets_per_cycle
				, repeat_count
				, seconds_per_bullet
				, sleep_per_cycle
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_CircularSector_01_1Direction::init()
		{
			mPivotPosition.scale( mStageConfig.GetBulletGenerateArea().size.width * 0.5f );
			mPivotPosition += mStageConfig.GetCenter();
		}

		void SpawnProcessor_CircularSector_01_1Direction::Enter( const Vec2& target_position )
		{
			mStep = eStep::Fire;

			mRemainTime = mSecondsPerBullet;

			mFireStartDirection = target_position - mPivotPosition;
			mFireStartDirection.normalize();
			mFireStartDirection.rotate( Vec2::ZERO, -mHalfRadianPerCycle );

			mCurrentFireCount = 0;
		}
		bool SpawnProcessor_CircularSector_01_1Direction::Update( float dt, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			if( eStep::Fire == mStep )
			{
				mRemainTime += dt;

				Vec2 temp_fire_direction = mFireStartDirection;

				while( mSecondsPerBullet <= mRemainTime )
				{
					temp_fire_direction.rotate( Vec2::ZERO, mRadianPerBullet * ( mCurrentFireCount % mBulletsPerCycle ) );

					out_spawn_info_container->push_back( SpawnInfo{
						mPivotPosition
						, temp_fire_direction
						} );

					++mCurrentFireCount;
					if( mRequiredBulletCount <= mCurrentFireCount )
					{
						break;
					}

					if( 0 == mCurrentFireCount % mBulletsPerCycle )
					{
						mStep = eStep::Sleep;
						mElapsedTime4Sleep = 0.f;
					}

					mRemainTime -= mSecondsPerBullet;
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
