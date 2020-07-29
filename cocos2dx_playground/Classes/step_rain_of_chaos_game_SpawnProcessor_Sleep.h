#pragma once

#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class SpawnProcessor_Sleep : public iSpawnProcessor
		{
		private:
			SpawnProcessor_Sleep( const float required_time );

		public:
			static SpawnProcessorUp Create( const float required_time );

			void Enter( const cocos2d::Vec2& target_position ) override;
			bool Update( float dt, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			const float mRequiredTime;
			float mElapsedTime;
		};
	}
}
