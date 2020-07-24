#pragma once

#include <memory>
#include <vector>

#include "math/Vec2.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageConfig;

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
			iSpawnProcessor( const StageConfig& stage_config ) : mStageConfig( stage_config ) {}

		public:
			virtual ~iSpawnProcessor() {}

			virtual void init() {}
			virtual bool Update( float dt, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) = 0;

		protected:
			const StageConfig& mStageConfig;
		};

		class TestSpawnProcessor : public iSpawnProcessor
		{
		private:
			TestSpawnProcessor( const StageConfig& stage_config );

		public:
			static SpawnProcessorUp Create( const StageConfig& stage_config );

			bool Update( float dt, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			float mElapsedTime;
		};
	}
}
