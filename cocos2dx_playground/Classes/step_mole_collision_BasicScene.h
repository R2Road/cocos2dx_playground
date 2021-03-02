#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_mole
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

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void collisionCheck();
			void updateDistance();

			void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			
			cocos2d::Node* mActorNode;
			cocos2d::Node* mBulletNode;
			cocos2d::Label* mDistanceLabel;
		};
	}
}
