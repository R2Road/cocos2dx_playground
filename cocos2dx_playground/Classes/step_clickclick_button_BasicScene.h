#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_clickclick
{
	namespace button
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Button : Basic"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onButton( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void updateView_TouchPosition( const cocos2d::Vec2 touch_position );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Label* mButtonStatusLabel;
			cocos2d::Label* mTouchInfoLabel;
		};
	}
}
