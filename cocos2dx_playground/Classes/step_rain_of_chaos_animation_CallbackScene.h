#pragma once

#include "2d/CCScene.h"

namespace step_rain_of_chaos
{
	namespace animation
	{
		class CallbackScene : public cocos2d::Scene
		{
		private:
			CallbackScene();

		public:
			static const char* getTitle() { return "Animation : Callback"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			void AnimationStartCallback();
			void AnimationEndCallback();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Action* mSequenceAction;
		};
	}
}
