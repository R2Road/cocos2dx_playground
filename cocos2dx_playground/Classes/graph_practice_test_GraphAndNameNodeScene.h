#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace graph_practice
{
	class GraphAndNameNode;
}

namespace graph_practice_test
{
	class GraphAndNameNodeScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		GraphAndNameNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Test : Graph And Name Node"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void update4Graph( float dt );
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		graph_practice::GraphAndNameNode* mGraphAndNameNode;
		float mElapsedTime;
	};
}
