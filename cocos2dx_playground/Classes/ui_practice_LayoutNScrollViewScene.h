#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace ui_practice
{
	class LayoutNScrollViewScene : public cocos2d::Scene
	{
	private:
		LayoutNScrollViewScene();

	public:
		static const char* getTitle() { return "Layout N ScrollView"; }
		static cocos2d::Scene* create();

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onDummyButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchEventType );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
