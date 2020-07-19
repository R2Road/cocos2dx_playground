#pragma once

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

		class iSpawnProcessor
		{
		public:
			virtual ~iSpawnProcessor() {}

			virtual bool Update( float dt, SpawnInfoContainer* out_spawn_info_container ) = 0;
		};

		class TestSpawnProcessor : public iSpawnProcessor
		{
		public:
			bool Update( float dt, SpawnInfoContainer* out_spawn_info_container ) override;
		};
	}
}
