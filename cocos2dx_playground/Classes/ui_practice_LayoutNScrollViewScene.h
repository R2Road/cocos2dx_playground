#pragma once

#include "2d/CCScene.h"

namespace ui_practice
{
	class LayoutNScrollViewScene : public cocos2d::Scene
	{
	public:
		LayoutNScrollViewScene();

		static const char* getTitle() { return "Layout N ScrollView"; }
		static cocos2d::Scene* create();

		bool init() override;
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
