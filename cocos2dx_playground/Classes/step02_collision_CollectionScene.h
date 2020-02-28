#pragma once

#include <list>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace cpg
{
	class CollisionComponent;
}

namespace step02
{
	namespace collision
	{
		class CollectionScene : public cocos2d::Scene
		{
		private:
			CollectionScene();

		public:
			static const char* getTitle() { return "Collision : Collection"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

			using cocos2d::Scene::addChild;
			void addChild( Node* child, int localZOrder, int tag ) override;
			void addChild( Node* child, int localZOrder, const std::string &name ) override;

			void removeChild( Node* child, bool cleanup = true ) override;
			void removeAllChildrenWithCleanup( bool cleanup ) override;

		private:
			void updateDistance();

			void onButton( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Vec2 mButtonMoveOffset;

			std::list<cpg::CollisionComponent*> mCollisionList;
		};
	}
}
