#pragma once

#include "2d/CCScene.h"

namespace step_flipflip
{
	namespace texture
	{
		class PList4ThisProjectScene : public cocos2d::Scene
		{
		private:
			PList4ThisProjectScene();

		public:
			static const char* getTitle() { return "Texture : PList 4 This Project"; }
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
