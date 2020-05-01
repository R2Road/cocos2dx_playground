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
		void CustomeUpdate( float dt );
		void updateLabel( cocos2d::Label* const label, const float elapsed_time );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cocos2d::Label* mLabel_forUpdate;
		cocos2d::Label* mLabel_forCustomeUpdate;
		cocos2d::Label* mLabel_forLambda;

		float mElapsedTime_forUpdate;
		float mElapsedTime_forCustomeUpdate;
		float mElapsedTime_forLambda;
	};
}
