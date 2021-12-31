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

namespace cocos_research_render
{
	class SequenceScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		SequenceScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Render Sequence"; }
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

		step_defender::game::TileMapNode* mTileMapNode;
	};
}
