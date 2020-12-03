#pragma once

#include "2d/CCScene.h"

namespace step_flipflip
{
	namespace game_test
	{
		class StageViewScene : public cocos2d::Scene
		{
		private:
			StageViewScene();

		public:
			static const char* getTitle() { return "Game Test : Stage View"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
