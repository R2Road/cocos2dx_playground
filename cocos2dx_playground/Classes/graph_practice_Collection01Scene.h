#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace graph_practice
{
	class GraphViewNode;

	class Collection01Scene : public cocos2d::Scene, public helper::BackToThePreviousScene
	{
	public:
		Collection01Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		static const char* getTitle() { return "Collection 01"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		bool init() override;
		void onEnter() override;
		void onExit() override;
		void update( float dt ) override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		float mElapsedTime;

		GraphViewNode* mGraphViewNode_1;
		GraphViewNode* mGraphViewNode_2;
		GraphViewNode* mGraphViewNode_3;
		GraphViewNode* mGraphViewNode_4;
	};
}
