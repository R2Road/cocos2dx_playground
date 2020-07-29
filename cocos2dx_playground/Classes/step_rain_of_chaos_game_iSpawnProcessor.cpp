#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		TestSpawnProcessor::TestSpawnProcessor( const StageConfig& stage_config ) : iSpawnProcessor( stage_config )
			, mElapsedTime( 0.f )
		{}

		SpawnProcessorUp TestSpawnProcessor::Create( const StageConfig& stage_config )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) TestSpawnProcessor( stage_config ) );
			return ret;
		}

		bool TestSpawnProcessor::Update( float dt, const cocos2d::Vec2& /*target_position*/, SpawnInfoContainer* out_spawn_info_container )
		{
			mElapsedTime += dt;
			if( 3.f > mElapsedTime )
			{
				return true;
			}
			else
			{
				out_spawn_info_container->push_back( SpawnInfo{ cocos2d::Vec2( 240.0f, 160.0f ), cocos2d::Vec2( 0.f, 1.f ) } );
				out_spawn_info_container->push_back( SpawnInfo{ cocos2d::Vec2( 240.0f, 160.0f ), cocos2d::Vec2( 0.f, -1.f ) } );
				out_spawn_info_container->push_back( SpawnInfo{ cocos2d::Vec2( 240.0f, 160.0f ), cocos2d::Vec2( 1.f, 0.f ) } );
				out_spawn_info_container->push_back( SpawnInfo{ cocos2d::Vec2( 240.0f, 160.0f ), cocos2d::Vec2( -1.f, 0.f ) } );
			}

			return false;
		}
	}
}
