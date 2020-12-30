#pragma once

#include "2d/CCScene.h"


namespace step_clickclick
{
	namespace game
	{
		class EffectViewNode;
	}

	namespace game_test
	{
		class EffectViewScene : public cocos2d::Scene
		{
		private:
			EffectViewScene();

		public:
			static const char* getTitle() { return "Game Test : Effect View"; }
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
			game::EffectViewNode* mEffectViewNode;
		};
	}
}
