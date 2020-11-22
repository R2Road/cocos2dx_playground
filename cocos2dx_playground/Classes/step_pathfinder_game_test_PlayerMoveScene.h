#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

#include "step_pathfinder_game_Constant.h"

namespace step_pathfinder
{
	namespace game
	{
		class TerrainViewer;
	}
}

namespace step_pathfinder
{
	namespace game_test
	{
		class PlayerMoveScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			PlayerMoveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Player Move"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void playerMove( const int move_x, const int move_y );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			
			game::TerrainViewer* mTerrainViewer;
			cocos2d::Node* mPlayer;
			game::TerrainPoint mPlayerPoint;
		};
	}
}
