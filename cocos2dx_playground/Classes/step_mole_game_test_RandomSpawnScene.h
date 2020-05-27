#pragma once

#include <memory>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	namespace game
	{
		using TargetManagerUp = std::unique_ptr<class TargetManager>;
		class StageNode;

		class StageTestScene : public cocos2d::Scene
		{
		private:
			StageTestScene();

		public:
			static const char* getTitle() { return "Game Test : Random Spawn"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			TargetManagerUp mTargetManager;
			StageNode* mStageNode;
		};
	}
}
