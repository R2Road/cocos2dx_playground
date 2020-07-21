#pragma once

#include <memory>
#include <vector>

#include "math/Vec2.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		struct SpawnInfo
		{
			cocos2d::Vec2 StartPosition = cocos2d::Vec2::ZERO;
			cocos2d::Vec2 MoveDirection = cocos2d::Vec2::ZERO;
		};
		using SpawnInfoContainer = std::vector<SpawnInfo>;

		using SpawnProcessorUp = std::unique_ptr<class iSpawnProcessor>;
		using SpawnProcessorContainer = std::vector<SpawnProcessorUp>;

		class iSpawnProcessor
		{
		protected:
			iSpawnProcessor() {}

		public:
			virtual ~iSpawnProcessor() {}

			virtual bool Update( float dt, SpawnInfoContainer* out_spawn_info_container ) = 0;
		};

		class TestSpawnProcessor : public iSpawnProcessor
		{
		private:
			TestSpawnProcessor();

		public:
			static SpawnProcessorUp Create();

			bool Update( float dt, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			float mElapsedTime;
		};
	}
}
