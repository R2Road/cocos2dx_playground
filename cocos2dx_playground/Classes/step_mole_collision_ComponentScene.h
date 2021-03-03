#pragma once

#include <list>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

namespace step_mole
{
	class CircleCollisionComponent;

	namespace collision
	{
		class ComponentScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			ComponentScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Collision : Component"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			cocos2d::Node* makeBullet( const bool collision_enable );

			void collisionCheck();
			void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Node* mActorNode;
			std::list<CircleCollisionComponent*> mCollisionComponentList;
		};
	}
}
