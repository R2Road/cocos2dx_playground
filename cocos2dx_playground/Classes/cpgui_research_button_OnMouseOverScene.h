#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
class EventListenerMouse;
NS_CC_END

namespace cpgui
{
	namespace research_button
	{
		class OnMouseOverScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			OnMouseOverScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Button Research : OnMouseOver"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
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
