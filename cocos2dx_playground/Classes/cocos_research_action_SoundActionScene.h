#pragma once

#include "helper_BackToThePreviousScene.h"
#include "2d/CCScene.h"

namespace cocos_research_action
{
	class SoundActionScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		SoundActionScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		~SoundActionScene();

		static const char* getTitle() { return "Sound Action"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cocos2d::Node* mTestNode_1;
		cocos2d::Node* mTestNode_2;

		cocos2d::Action* mTestAction_1;
		cocos2d::Action* mTestAction_2;
	};
}
