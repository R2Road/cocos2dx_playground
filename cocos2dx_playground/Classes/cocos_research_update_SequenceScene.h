#pragma once

#include <string>

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace cocos_research_update
{
	class SequenceScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		SequenceScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Update Sequence"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;
		void update( float dt ) override;

	private:
		void test_Update( float dt );
		void test_UpdateOnce( float dt );
		void test_UpdateEnd( float dt );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cocos2d::Label* mLabel4Log;
		std::string mUpdateLog;

		cocos2d::Node* mTestNode;

		bool mbInputBlock;
	};
}
