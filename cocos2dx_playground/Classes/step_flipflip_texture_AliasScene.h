#pragma once

#include "2d/CCScene.h"

namespace step_flipflip
{
	namespace texture
	{
		class AliasScene : public cocos2d::Scene
		{
		private:
			AliasScene();

		public:
			static const char* getTitle() { return "Texture : Alias"; }
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
