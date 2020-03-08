#pragma once

#include "cocos2d.h"

namespace step_clickclick
{
	namespace game
	{

		class Stage;

		class StageTestScene : public cocos2d::Scene
		{
		private:
			StageTestScene();

		public:
			static const char* getTitle() { return "Game : Stage Test"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			Stage* mStageNode;
		};
	}
}
