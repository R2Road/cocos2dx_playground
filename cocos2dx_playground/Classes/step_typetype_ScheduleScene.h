#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_typetype
{
	class ScheduleScene : public cocos2d::Scene
	{
	private:
		struct ScheduleView
		{
			cocos2d::Label* ViewLabel = nullptr;
			float Elapsedtime = 0.f;
		};

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
		void updateLabel( ScheduleView& schedule_view );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		ScheduleView mScheduleView_forUpdate;
		ScheduleView mScheduleView_forCustomeUpdate;
		ScheduleView mScheduleView_forLambda;
	};
}
