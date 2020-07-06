#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace step_rain_of_chaos
{
	namespace game_test
	{
		class BulletLifeComponentScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		private:
			BulletLifeComponentScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Bullet Life Component"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateMoveSpeedView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Vec2 mCenter;
			cocos2d::Rect mBulletLifeArea;
			cocos2d::Rect mBulletGenerateArea;
			int mCurrentMoveSpeed;
		};
	}
}
