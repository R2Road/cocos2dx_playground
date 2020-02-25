#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step02
{
	namespace collision
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Collision : Basic"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateDistance();

			void onButton( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Vec2 mButtonMoveOffset;
		};
	}
}
