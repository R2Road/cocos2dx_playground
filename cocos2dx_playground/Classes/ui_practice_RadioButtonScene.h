#pragma once

#include "2d/CCScene.h"
#include "math/CCGeometry.h"
#include "ui/UIWidget.h"
#include "ui/UILayoutParameter.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
namespace ui
{
	class Layout;
}
NS_CC_END

namespace ui_practice
{
	class RadioButtonScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	public:
		RadioButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		static const char* getTitle() { return "Radio Button"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		bool init() override;
		void onEnter() override;
		void onExit() override;

	private:
		void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchEventType );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cocos2d::ui::Layout* mLayout;
		bool mDirection;
	};
}
