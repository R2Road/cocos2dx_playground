#pragma once

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

namespace step_rain_of_chaos
{
	namespace game_test
	{
		class BackgroundNodeScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			BackgroundNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Background Node"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateAmountView();
			void updateBackgroundNode();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			int mHorizontalAmount;
			int mVerticalAmount;
		};
	}
}
