#pragma once

#include <list>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	class CircleCollisionComponent;

	namespace collision
	{
		class ComponentScene : public cocos2d::Scene
		{
		private:
			ComponentScene();

		public:
			static const char* getTitle() { return "Collision : Component"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			cocos2d::Node* makeBullet( const bool collision_enable );

			void collisionCheck();
			void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			std::list<CircleCollisionComponent*> mCollisionComponentList;
		};
	}
}
