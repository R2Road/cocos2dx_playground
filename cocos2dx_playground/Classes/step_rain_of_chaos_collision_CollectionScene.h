#pragma once

#include <list>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	class CircleCollisionComponent;
}

namespace step_rain_of_chaos
{
	namespace collision
	{
		class CollectionScene : public cocos2d::Scene
		{
		public:
			using ContainerT = std::list<step_mole::CircleCollisionComponent*>;

		private:
			CollectionScene();

		public:
			static const char* getTitle() { return "Collision : Collection"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void update( float dt ) override;
			void onExit() override;

			using cocos2d::Scene::addChild;
			void addChild( cocos2d::Node* child, int localZOrder, int tag ) override;
			void addChild( cocos2d::Node* child, int localZOrder, const std::string &name ) override;

			void removeChild( cocos2d::Node* child, bool cleanup = true ) override;
			void removeAllChildrenWithCleanup( bool cleanup ) override;

		private:
			void addCollision( cocos2d::Node* child );
			void removeCollision( cocos2d::Node* child );

			void makeBullet();
			void removeBullet();

			void updateBulletCountView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			ContainerT mCollisionCollection;
		};
	}
}
