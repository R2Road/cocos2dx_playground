#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	class ScheduleView;

	class ScheduleScene : public cocos2d::Scene
	{
	private:
		ScheduleScene();

	public:
		static const char* getTitle() { return "Schedule"; }
		static cocos2d::Scene* create();

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

		void update( float dt ) override;

	private:
		void customeUpdate( float dt );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		ScheduleView* mScheduleView4Update;
		ScheduleView* mScheduleView4CustomeUpdate;
		ScheduleView* mScheduleView4Lambda;
	};
}
