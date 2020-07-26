#include "step_rain_of_chaos_game_SpawnProcessor_CircularSector_01_1Direction.h"

#include <algorithm>

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		SpawnProcessor_CircularSector_01_1Direction::SpawnProcessor_CircularSector_01_1Direction( const StageConfig& stage_config, const float degree_per_cycle, const int bullets_per_cycle, const int repeat_count, const float total_time ) : iSpawnProcessor( stage_config )
			, mHalfRadianPerCycle( CC_DEGREES_TO_RADIANS( degree_per_cycle * 0.5f ) )
			, mBulletsPerCycle( std::max( 1, bullets_per_cycle ) )
			, mRequiredBulletCount( std::max( 1, mBulletsPerCycle * repeat_count ) )
			, mRadianPerBullet( CC_DEGREES_TO_RADIANS( degree_per_cycle / std::max( 1, mBulletsPerCycle - 1 ) ) )
			, mSecondsPerBullet( total_time / mRequiredBulletCount )

			, mRemainTime( 0.f )
			, mPivotPosition( Vec2::UNIT_Y )
			, mFireStartDirection()
			, mCurrentFireCount( 0 )
		{}

		SpawnProcessorUp SpawnProcessor_CircularSector_01_1Direction::Create( const StageConfig& stage_config, const float degree_per_cycle, const int bullets_per_cycle, const int repeat_count, const float total_time )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_CircularSector_01_1Direction( stage_config, degree_per_cycle, bullets_per_cycle, repeat_count, total_time ) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_CircularSector_01_1Direction::init()
		{
			mPivotPosition.scale( mStageConfig.GetBulletGenerateArea().size.width * 0.5f );
			mPivotPosition += mStageConfig.GetCenter();

			mFireStartDirection.set( 0.f, -1.f );
		}
		bool SpawnProcessor_CircularSector_01_1Direction::Update( float dt, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
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

				mRemainTime -= mSecondsPerBullet;
			}

			return mRequiredBulletCount > mCurrentFireCount;
		}
	}
}
