#pragma once

#include <memory>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	namespace game
	{
		using TargetManagerUp = std::unique_ptr<class TargetManager>;
		class StageView;
	}

	namespace game_test
	{
		class HitTestScene : public cocos2d::Scene
		{
		private:
			HitTestScene();

		public:
			static const char* getTitle() { return "Game Test : Hit Test"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateSpawnTargetCountView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::TargetManagerUp mTargetManager;
			game::StageView* mStageView;

			int mCurrentSpawnTargetCount;
		};
	}
}
