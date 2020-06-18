#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
class EventListenerMouse;
NS_CC_END

namespace ui_research
{
	class ButtonWithOnMouseOverScene : public cocos2d::Scene, public helper::BackToThePreviousScene
	{
	private:
		ButtonWithOnMouseOverScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Button With OnMouseOver"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		bool init() override;
		void onEnter() override;
		void onExit() override;

	private:
		void onButton( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void updateMouseStatusView( bool is_over );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		cocos2d::EventListenerMouse* mMouseListener;
		bool mbOnMouseOver;
	};
}
