#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
class EventListenerMouse;
NS_CC_END

namespace ui_research
{
	namespace button_research
	{
		class MouseRightClickScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			MouseRightClickScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Button Research : Mouse Right Click"; }
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
}
