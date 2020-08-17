#pragma once

#include "step_rain_of_chaos_game_iEnemyProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class EnemyProcessor_Sleep : public iEnemyProcessor
		{
		private:
			EnemyProcessor_Sleep( const float limit_time );

		public:

			static EnemyProcessorUp Create( const float limit_time );

			void Enter() override;
			bool Update( const float delta_time ) override;

		private:
			const float mLimitTime;
			float mElapsedTime;
		};
	}
}
