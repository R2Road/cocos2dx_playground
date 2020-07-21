#include "step_rain_of_chaos_game_SpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		TestSpawnProcessor::TestSpawnProcessor() : mElapsedTime( 0.f )
		{}

		SpawnProcessorUp TestSpawnProcessor::Create()
		{
			SpawnProcessorUp ret( new ( std::nothrow ) TestSpawnProcessor );
			return ret;
		}

		bool TestSpawnProcessor::Update( float dt, SpawnInfoContainer* out_spawn_info_container )
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
