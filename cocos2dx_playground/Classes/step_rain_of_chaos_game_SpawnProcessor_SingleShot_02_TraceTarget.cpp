#include "step_rain_of_chaos_game_SpawnProcessor_SingleShot_02_TraceTarget.h"

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
		SpawnProcessor_SingleShot_02_TraceTarget::SpawnProcessor_SingleShot_02_TraceTarget( const StageConfig& stage_config, const int repeat_count, const float sleep_per_cycle ) : iSpawnProcessor( stage_config )
			, mRequiredCycle( std::max( 1, repeat_count ) )
			, mSleepPerCycle( sleep_per_cycle )

			, mStep( eStep::Fire )

			, mStartPosition( Vec2::UNIT_Y )
			, mCurrentFireCycle( 0 )

			, mElapsedTime4Sleep( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_SingleShot_02_TraceTarget::Create( const StageConfig& stage_config, const int repeat_count, const float sleep_per_cycle )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_SingleShot_02_TraceTarget(
				stage_config
				, repeat_count
				, sleep_per_cycle
			) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_SingleShot_02_TraceTarget::Enter( const cocos2d::Vec2& start_position, const Vec2& target_position )
		{
			mStep = eStep::Fire;

			mStartPosition = start_position;

			mCurrentFireCycle = 0;
		}
		bool SpawnProcessor_SingleShot_02_TraceTarget::Update( float dt, const Vec2& target_position, SpawnInfoContainer* out_spawn_info_container )
		{
			if( eStep::Fire == mStep )
			{
				auto fire_direction = target_position - mStartPosition;
				fire_direction.normalize();

				out_spawn_info_container->push_back( SpawnInfo{
					mStartPosition
					, fire_direction
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
