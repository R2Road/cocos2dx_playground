#pragma once

#include <list>

#include "2d/CCScene.h"

#include "algorithm_practice_floodfill_Grid4TileMap.h"
#include "helper_BackToThePreviousScene.h"

#include "cpg_Point.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_loophero
{
	class SquareScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		SquareScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Square"; }
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

		algorithm_practice_floodfill::Grid4TileMap mGrid4TileMap;
		step_defender::game::TileMapNode* mTileMapNode;

		std::list<cpg::Point> mRoad;
	};
}
