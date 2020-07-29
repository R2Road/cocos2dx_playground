#include "step_rain_of_chaos_game_SpawnProcessor_CircularSector_01_2Direction.h"

#include <algorithm>

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		SpawnProcessor_CircularSector_01_2Direction::SpawnProcessor_CircularSector_01_2Direction(
			const StageConfig& stage_config
			, const bool rotate_direction_left
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float total_time
		) : iSpawnProcessor( stage_config )
			, mHalfRadianPerCycle( CC_DEGREES_TO_RADIANS( degree_per_cycle * 0.5f ) * ( rotate_direction_left ? 1 : -1 ) )
			, mBulletsPerCycle( std::max( 1, bullets_per_cycle ) )
			, mRequiredBulletCount( std::max( 1, mBulletsPerCycle * repeat_count ) )
			, mRadianPerBullet( CC_DEGREES_TO_RADIANS( degree_per_cycle / std::max( 1, mBulletsPerCycle - 1 ) ) * ( rotate_direction_left ? 1 : -1 ) )
			, mSecondsPerBullet( total_time / mRequiredBulletCount )

			, mRemainTime( 0.f )
			, mPivotPosition( Vec2::UNIT_Y )
			, mPivotDirection()
			, mCurrentFireStartDirection()
			, mCurrentRadianPerBullet( 0.f )
			, mCurrentFireCount( 0 )
		{}

		SpawnProcessorUp SpawnProcessor_CircularSector_01_2Direction::Create(
			const StageConfig& stage_config
			, const bool rotate_direction_left
			, const float degree_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float total_time
		)
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_CircularSector_01_2Direction( stage_config, rotate_direction_left, degree_per_cycle, bullets_per_cycle, repeat_count, total_time ) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_CircularSector_01_2Direction::init()
		{
			mPivotPosition.scale( mStageConfig.GetBulletGenerateArea().size.width * 0.5f );
			mPivotPosition += mStageConfig.GetCenter();
		}

		void SpawnProcessor_CircularSector_01_2Direction::Enter( const Vec2& target_position )
		{
			mRemainTime = mSecondsPerBullet;

			mPivotDirection = target_position - mPivotPosition;
			mPivotDirection.normalize();

			mCurrentFireStartDirection = mPivotDirection;
			mCurrentFireStartDirection.rotate( Vec2::ZERO, -mHalfRadianPerCycle );

			mCurrentRadianPerBullet = mRadianPerBullet;
			mCurrentFireCount = 0;
		}
		bool SpawnProcessor_CircularSector_01_2Direction::Update( float dt, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			mRemainTime += dt;

			Vec2 temp_fire_direction = mCurrentFireStartDirection;

			while( mSecondsPerBullet <= mRemainTime )
			{
				temp_fire_direction.rotate( Vec2::ZERO, mCurrentRadianPerBullet * ( mCurrentFireCount % mBulletsPerCycle ) );

				out_spawn_info_container->push_back( SpawnInfo{
					mPivotPosition
					, temp_fire_direction
				} );

				++mCurrentFireCount;
				if( 0 == mCurrentFireCount % mBulletsPerCycle )
				{
					mCurrentFireStartDirection = mPivotDirection;

					const int temp_direction = ( mCurrentFireCount / mBulletsPerCycle ) & 1 ? -1 : 1;
					mCurrentFireStartDirection.rotate( Vec2::ZERO, -mHalfRadianPerCycle * temp_direction ); // check odd number
					mCurrentRadianPerBullet = mRadianPerBullet * temp_direction;
				}

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
