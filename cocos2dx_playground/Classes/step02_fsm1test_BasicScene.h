#pragma once

#include "helper_BackToThePreviousScene.h"
#include "2d/CCScene.h"

#include "fsm1_Machine.h"

namespace step02
{
	namespace fsm1test
	{
		class BasicScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "FSM1 Test : Basic"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void update( float dt ) override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			fsm1::Machine mFSMMachine;
		};
	}
}
