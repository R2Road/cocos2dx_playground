#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

#include "algorithm_practice_floodfill_Constant.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace algorithm_practice_floodfill
{
	class DirectionCellScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		DirectionCellScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Direction Cell"; }
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

		DirectionCell mDirectionCell;
		cocos2d::Label* mDirectionView;
	};
}
