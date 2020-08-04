#include "step_rain_of_chaos_game_SpawnProcessor_SingleShot_01.h"

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
		SpawnProcessor_SingleShot_01::SpawnProcessor_SingleShot_01( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config, const int repeat_count, const float sleep_per_cycle ) :
			iSpawnProcessor( stage_config, spawn_processor_config )
			, mRequiredCycle( std::max( 1, repeat_count ) )
			, mSleepPerCycle( sleep_per_cycle )

			, mStep( eStep::Fire )

			, mStartPosition()
			, mTargetPosition()
			, mFireStartDirection()
			, mCurrentFireCycle( 0 )

			, mElapsedTime4Sleep( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_SingleShot_01::Create( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config, const int repeat_count, const float sleep_per_cycle )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_SingleShot_01(
				stage_config
				, spawn_processor_config
				, repeat_count
				, sleep_per_cycle
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_SingleShot_01::Enter( const Vec2& start_position, const Vec2& target_position )
		{
			mStep = eStep::Fire;

			mStartPosition = start_position;
			mTargetPosition = target_position;

			mFireStartDirection = mTargetPosition - mStartPosition;
			mFireStartDirection.normalize();

			mCurrentFireCycle = 0;
		}
		bool SpawnProcessor_SingleShot_01::Update( const float dt, const Vec2& start_position, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
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
				}

				out_spawn_info_container->push_back( SpawnInfo{
					mStartPosition
					, mFireStartDirection
				} );

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
