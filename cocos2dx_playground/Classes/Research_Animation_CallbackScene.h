#pragma once

#include "cocos2d.h"

namespace research
{
	namespace animation
	{
		class CallbackScene : public cocos2d::Scene
		{
		private:
			CallbackScene();

		public:
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			void AnimationEndCallback();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Action* mAction_Animation_Run_Sequence;
		};
	}
}
