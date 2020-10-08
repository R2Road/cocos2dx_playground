#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "step_rain_of_chaos_input_KeyCodeCollector.h"
#include "fsm1_Machine.h"

namespace step02
{
	namespace fsm1test
	{
		class AnimationControlScene : public cocos2d::Scene
		{
		private:
			AnimationControlScene();

		public:
			static const char* getTitle() { return "FSM1 Test : Animation Control"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void update( float dt ) override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;
			fsm1::Machine mFSMMachine;
		};
	}
}
