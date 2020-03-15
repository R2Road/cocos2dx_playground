#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	namespace game
	{
		class TitleScene : public cocos2d::Scene
		{
		private:
			TitleScene();

		public:
			static const char* getTitle() { return "Game : Type Type"; }
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
