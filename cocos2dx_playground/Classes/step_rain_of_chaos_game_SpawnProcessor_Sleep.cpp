#include "step_rain_of_chaos_game_SpawnProcessor_Sleep.h"

#include "step_rain_of_chaos_game_StageConfig.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		SpawnProcessor_Sleep::SpawnProcessor_Sleep( const float required_time ) : iSpawnProcessor( StageConfig(), SpawnProcessorConfig() )
			, mRequiredTime( required_time )
			, mElapsedTime( 0.f )
		{}

		SpawnProcessorUp SpawnProcessor_Sleep::Create( const float required_time )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) SpawnProcessor_Sleep( required_time ) );
			ret->init();
			return ret;
		}

		void SpawnProcessor_Sleep::Enter( const Vec2& /*start_position*/, const Vec2& /*target_position*/ )
		{
			mElapsedTime = 0.f;
		}
		bool SpawnProcessor_Sleep::Update( const float dt, const Vec2& /*start_position*/, const Vec2& /*target_position*/, SpawnInfoContainer* /*out_spawn_info_container*/ )
		{
			mElapsedTime += dt;
			return mRequiredTime > mElapsedTime;
		}
	}
}
