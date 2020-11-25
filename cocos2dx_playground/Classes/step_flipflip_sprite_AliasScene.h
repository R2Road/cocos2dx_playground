#pragma once

#include "2d/CCScene.h"

namespace step_flipflip
{
	namespace sprite
	{
		class AliasScene : public cocos2d::Scene
		{
		public:
			AliasScene();

			static const char* getTitle() { return "Sprite : Alias"; }
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
