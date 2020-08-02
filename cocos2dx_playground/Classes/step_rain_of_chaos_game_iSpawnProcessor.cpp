#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		TestSpawnProcessor::TestSpawnProcessor( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config )
			: iSpawnProcessor( stage_config, spawn_processor_config )
			, mElapsedTime( 0.f )
		{}

		SpawnProcessorUp TestSpawnProcessor::Create( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config )
		{
			SpawnProcessorUp ret( new ( std::nothrow ) TestSpawnProcessor( stage_config, spawn_processor_config ) );
			return ret;
		}

		bool TestSpawnProcessor::Update( const float dt, const cocos2d::Vec2& /*start_position*/, const cocos2d::Vec2& /*target_position*/, SpawnInfoContainer* out_spawn_info_container )
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
