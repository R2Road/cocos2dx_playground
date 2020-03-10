#pragma once

#include "2d/CCScene.h"

namespace step_clickclick
{
	namespace animation
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Animation : Basic"; }
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
