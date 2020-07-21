#pragma once

#include <memory>

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"
#include "step_rain_of_chaos_game_StageConfig.h"
#include "step_rain_of_chaos_game_SpawnProcessor.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		using BulletManagerUp = std::unique_ptr<class BulletManager>;
		class StageNode;
	}

	namespace game_test
	{
		class SpawnProcessorScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		private:
			SpawnProcessorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Spawn Processor"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForSpawnProcessor( float dt );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::StageConfig mStageConfig;
			game::BulletManagerUp mBulletManager;
			game::StageNode* mStageNode;

			game::SpawnProcessorContainer mSpawnProcessorContainer;
			game::SpawnProcessorContainer::iterator mCurrentSpawnProcessor;
		};
	}
}
