#pragma once

#include <vector>

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace graph_practice
{
	class GraphViewNode;
}

namespace graph_practice_test
{
	class GraphViewNodeScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		GraphViewNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Test : Graph View Node"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
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

		std::vector<graph_practice::GraphViewNode*> mGraphViewNodeContainer;
	};
}
