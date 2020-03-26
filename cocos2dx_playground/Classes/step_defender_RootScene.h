#pragma once

#include "2d/CCScene.h"

namespace step_defender
{
	class RootScene : public cocos2d::Scene
	{
	public:
		RootScene();

		static const char* getTitle() { return "Step : Defender"; }
		static cocos2d::Scene* create();

		bool init() override;
		void onEnter() override;
		void onExit() override;

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
