#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	namespace game
	{
		class LetterViewNode;
	}

	namespace game_test
	{
		class LetterViewScene : public cocos2d::Scene	
		{
		private:
			LetterViewScene();

		public:
			static const char* getTitle() { return "Game Test : Letter View"; }
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

			game::LetterViewNode* mLetterViewNode;
		};
	}
}
