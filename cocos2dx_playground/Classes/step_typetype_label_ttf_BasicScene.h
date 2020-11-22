#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	namespace label_ttf
	{
		class BasicScene : public cocos2d::Scene
		{
		public:
			BasicScene();

			static const char* getTitle() { return "Label TTF"; }
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