#include "step_rain_of_chaos_game_SpawnProcessor_MultipleShot_02_Line.h"

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
		SpawnProcessor_MultipleShot_02_Line::SpawnProcessor_MultipleShot_02_Line(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const float range_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float sleep_per_cycle
		) : iSpawnProcessor( stage_config, spawn_processor_config )
			, mHalfRangePerCycle( range_per_cycle * 0.5f )
			, mBulletsPerCycle( std::max( 2, bullets_per_cycle ) )
			, mRequiredCycle( std::max( 1, repeat_count ) )
			, mRangePerBullet( range_per_cycle / std::max( 1, mBulletsPerCycle - 1 ) )
			, mSleepPerCycle( sleep_per_cycle )

			, mStep( eStep::Fire )

			, mStartPosition()
			, mTargetPosition()
			, mFireDirection()
			, mFireStartPosition()
			, mFireOffset()
			, mCurrentFireCycle( 0 )

			, mElapsedTime4Sleep( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_MultipleShot_02_Line::Create(
			const StageConfig& stage_config
			, const SpawnProcessorConfig& spawn_processor_config
			, const float range_per_cycle
			, const int bullets_per_cycle
			, const int repeat_count
			, const float sleep_per_cycle
		)
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_MultipleShot_02_Line(
				stage_config
				, spawn_processor_config
				, range_per_cycle, bullets_per_cycle
				, repeat_count
				, sleep_per_cycle
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_MultipleShot_02_Line::Enter( const Vec2& start_position, const Vec2& target_position )
		{
			mStep = eStep::Fire;

			mStartPosition = start_position;
			mTargetPosition = target_position;

			mFireDirection = mTargetPosition - mStartPosition;
			mFireDirection.normalize();

			Vec2 temp;

			temp = mFireDirection;
			temp.rotate( Vec2::ZERO, CC_DEGREES_TO_RADIANS( 90 ) );			

			mFireStartPosition = temp;
			mFireStartPosition.scale( mHalfRangePerCycle );
			mFireStartPosition += mStartPosition;

			mFireOffset = -temp;
			mFireOffset.scale( mRangePerBullet );

			mCurrentFireCycle = 0;
		}
		bool SpawnProcessor_MultipleShot_02_Line::Update( const float dt, const Vec2& start_position, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
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
					mFireDirection = mTargetPosition - mStartPosition;
					mFireDirection.normalize();

					Vec2 temp;

					temp = mFireDirection;
					temp.rotate( Vec2::ZERO, CC_DEGREES_TO_RADIANS( 90 ) );

					mFireStartPosition = temp;
					mFireStartPosition.scale( mHalfRangePerCycle );
					mFireStartPosition += mStartPosition;

					mFireOffset = -temp;
					mFireOffset.scale( mRangePerBullet );
				}

				Vec2 fire_position;
				for( int i = 0; mBulletsPerCycle > i; ++i )
				{
					fire_position = mFireStartPosition;
					fire_position += ( mFireOffset * i );

					out_spawn_info_container->push_back( SpawnInfo{
						fire_position
						, mFireDirection
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
