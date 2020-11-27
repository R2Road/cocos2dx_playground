#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	namespace sprite
	{
		class AnchorPointScene : public cocos2d::Scene
		{
		public:
			AnchorPointScene();

			static const char* getTitle() { return "Sprite : AnchorPoint"; }
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
