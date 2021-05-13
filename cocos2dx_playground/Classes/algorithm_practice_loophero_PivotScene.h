#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_loophero
{
	class PivotScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		PivotScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Pivot"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		step_defender::game::TileMapNode* mTileMapNode;
	};
}
