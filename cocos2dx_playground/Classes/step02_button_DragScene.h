#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step02
{
	namespace button
	{
		class DragScene : public cocos2d::Scene
		{
		private:
			DragScene();

		public:
			static const char* getTitle() { return "Button : Drag"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onButton( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Vec2 mButtonMoveOffset;
		};
	}
}
