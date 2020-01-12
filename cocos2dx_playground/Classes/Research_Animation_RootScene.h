#pragma once

#include "cocos2d.h"

namespace research
{
	namespace animation
	{
		class RootScene : public cocos2d::Scene
		{
		private:
			RootScene();

		public:
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			bool mInputBlocked;
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
