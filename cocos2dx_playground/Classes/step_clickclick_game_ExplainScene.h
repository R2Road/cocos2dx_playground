#pragma once

#include "2d/CCScene.h"

namespace step_clickclick
{
	namespace game
	{
		class ExplainScene : public cocos2d::Scene
		{
		private:
			ExplainScene();

		public:
			static const char* getTitle() { return "Game : Explain"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
