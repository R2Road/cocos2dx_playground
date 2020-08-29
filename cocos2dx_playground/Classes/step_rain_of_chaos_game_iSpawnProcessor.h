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
		using SpawnProcessorPackage = std::vector<SpawnProcessorUp>;

		struct SpawnProcessorConfig
		{
			bool UpdateStartPosition = false;
			bool UpdateTargetPosition = false;
		};

		class iSpawnProcessor
		{
		protected:
			iSpawnProcessor( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config ) : mStageConfig( stage_config ), mSpawnProcessorConfig( spawn_processor_config ) {}

		public:
			virtual ~iSpawnProcessor() {}

			virtual void init() {}
			virtual void Enter( const cocos2d::Vec2& /*start_position*/, const cocos2d::Vec2& /*target_position*/ ) {}
			virtual bool Update( const float dt, const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) = 0;

		protected:
			const StageConfig& mStageConfig;
			const SpawnProcessorConfig mSpawnProcessorConfig;
		};

		class TestSpawnProcessor : public iSpawnProcessor
		{
		private:
			TestSpawnProcessor( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config );

		public:
			static SpawnProcessorUp Create( const StageConfig& stage_config, const SpawnProcessorConfig& spawn_processor_config );

			bool Update( const float dt, const cocos2d::Vec2& start_position, const cocos2d::Vec2& target_position, SpawnInfoContainer* out_spawn_info_container ) override;

		private:
			float mElapsedTime;
		};
	}
}
