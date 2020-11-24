#pragma once

#include <memory>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

namespace step_mole
{
	namespace game
	{
		using TargetManagerUp = std::unique_ptr<class TargetManager>;
		class StageNode;
	}

	namespace game_test
	{
		class SpawnScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			SpawnScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Spawn"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateSpawnTargetCountView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::TargetManagerUp mTargetManager;
			game::StageNode* mStageNode;

			int mCurrentSpawnTargetCount;
		};
	}
}
