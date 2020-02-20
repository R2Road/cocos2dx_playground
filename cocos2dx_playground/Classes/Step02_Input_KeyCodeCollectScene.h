#pragma once

#include <vector>

#include "cocos2d.h"

namespace step02
{
	namespace input
	{
		class KeyCodeCollectScene : public cocos2d::Scene
		{
		private:
			KeyCodeCollectScene();

		public:
			static const char* getTitle() { return "Input : KeyCode Collect"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
