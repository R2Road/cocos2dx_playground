#include "step_rain_of_chaos_game_EnemyProcessor_Sleep.h"

#include <new>

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		EnemyProcessor_Sleep::EnemyProcessor_Sleep( const float limit_time ) : iEnemyProcessor( StageConfig{}, nullptr, nullptr )
			, mLimitTime( limit_time )
			, mElapsedTime( 0.f )
		{}

		EnemyProcessorUp EnemyProcessor_Sleep::Create( const float limit_time )
		{
			EnemyProcessorUp ret( new (std::nothrow) EnemyProcessor_Sleep( limit_time ) );
			ret->init();
			return ret;
		}

		void EnemyProcessor_Sleep::Enter()
		{
			mElapsedTime = 0.f;
		}

		bool EnemyProcessor_Sleep::Update( const float delta_time )
		{
			if( mLimitTime <= mElapsedTime )
			{
				return false;
			}

			mElapsedTime += delta_time;
			return mLimitTime > mElapsedTime;
		}
	}
}
