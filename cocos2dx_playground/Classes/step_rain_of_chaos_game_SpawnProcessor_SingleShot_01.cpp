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
		SpawnProcessor_SingleShot_01::SpawnProcessor_SingleShot_01( const StageConfig& stage_config, const int repeat_count, const float sleep_per_cycle ) : iSpawnProcessor( stage_config )
			, mRequiredCycle( std::max( 1, repeat_count ) )
			, mSleepPerCycle( sleep_per_cycle )

			, mStep( eStep::Fire )

			, mPivotPosition( Vec2::UNIT_Y )
			, mFireStartDirection()
			, mCurrentFireCycle( 0 )

			, mElapsedTime4Sleep( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_SingleShot_01::Create( const StageConfig& stage_config, const int repeat_count, const float sleep_per_cycle )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_SingleShot_01(
				stage_config
				, repeat_count
				, sleep_per_cycle
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_SingleShot_01::init()
		{
			mPivotPosition.scale( mStageConfig.GetBulletGenerateArea().size.width * 0.5f );
			mPivotPosition += mStageConfig.GetCenter();
		}

		void SpawnProcessor_SingleShot_01::Enter( const Vec2& target_position )
		{
			mStep = eStep::Fire;

			mFireStartDirection = target_position - mPivotPosition;
			mFireStartDirection.normalize();

			mCurrentFireCycle = 0;
		}
		bool SpawnProcessor_SingleShot_01::Update( float dt, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			if( eStep::Fire == mStep )
			{
				out_spawn_info_container->push_back( SpawnInfo{
					mPivotPosition
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
