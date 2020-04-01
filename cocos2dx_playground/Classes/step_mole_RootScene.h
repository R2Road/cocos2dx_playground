#pragma once

#include "2d/CCScene.h"

namespace step_mole
{
	class RootScene : public cocos2d::Scene
	{
	public:
		RootScene();

		static const char* getTitle() { return "Step : Mole"; }
		static cocos2d::Scene* create();

		void onEnter() override;
		void onExit() override;

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
