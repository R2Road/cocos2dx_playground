#pragma once

#include "step_rain_of_chaos_game_iSpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class SpawnProcessor_01_CircleShape_OutToIn : public iSpawnProcessor
		{
		private:
			SpawnProcessor_01_CircleShape_OutToIn( const StageConfig& stage_config );

		public:
			static SpawnProcessorUp Create( const StageConfig& stage_config );

			void init() override;
			bool Update( float dt, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			float mRemainTime;
			cocos2d::Vec2 mCurrentStartPosition;
			int mCurrentFireCount;
		};
	}
}
