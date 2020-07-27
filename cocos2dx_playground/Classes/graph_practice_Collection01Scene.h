#pragma once

#include <vector>

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace graph_practice
{
	class GraphViewNode;

	class Collection01Scene : public cocos2d::Scene, private helper::BackToThePreviousScene
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
		void UpdateTimeScaleView();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		float mTimeScale;
		float mElapsedTime;
		bool mbPause;

		std::vector<GraphViewNode*> mGraphViewNodeContainer;
	};
}
