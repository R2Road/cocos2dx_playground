#pragma once

#include <list>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "cpg_input_KeyCodeCollector.h"

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
			void update( float dt ) override;
			void onExit() override;

			using cocos2d::Scene::addChild;
			void addChild( cocos2d::Node* child, int localZOrder, int tag ) override;
			void addChild( cocos2d::Node* child, int localZOrder, const std::string &name ) override;

			void removeChild( cocos2d::Node* child, bool cleanup = true ) override;
			void removeAllChildrenWithCleanup( bool cleanup ) override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			std::list<cpg::CollisionComponent*> mCollisionList;

			cpg::input::KeyCodeCollector mKeyCodeCollector;
			int mMoveSpeed;
		};
	}
}
