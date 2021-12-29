#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace algorithm_practice_astar
{
	class CostMapNode;

	class CostMapNodeScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		CostMapNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Cost Map Node"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void requestMoveIndicator( const int move_x, const int move_y );
		void moveIndicator( const int new_x, const int new_y );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		CostMapNode* mCostMapNode;
		cocos2d::Node* mIndicatorNode;

		int mIndicatorPointX;
		int mIndicatorPointY;
	};
}
