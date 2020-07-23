#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace step_mole
{
	namespace game_test
	{
		class ObjectActionScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			ObjectActionScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Object Action"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateLifeTimeView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			int mCurrentLifeTime;
		};
	}
}
