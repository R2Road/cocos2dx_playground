#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

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
		void update( float dt ) override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		float mElapsedTime;
	};
}
