#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

#include "cpg_Direction4.h"

NS_CC_BEGIN
	class DrawNode;
	class Label;
NS_CC_END

namespace algorithm_practice_floodfill
{
	class Direction4Scene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		Direction4Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Direction4"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void updateDirectionView();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cpg::Direction4 mDirection4;
		cocos2d::Label* mDirectionView;
		cocos2d::DrawNode* mDirectionIndicator;
	};
}
