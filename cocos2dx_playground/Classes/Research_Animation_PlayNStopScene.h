#pragma once

#include "cocos2d.h"

namespace research
{
	namespace animation
	{
		class PlayNStopScene : public cocos2d::Scene
		{
		private:
			PlayNStopScene();

		public:
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Action* mAction_Animation_Run_Repeat;
		};
	}
}
