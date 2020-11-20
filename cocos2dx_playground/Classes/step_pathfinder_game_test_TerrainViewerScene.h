#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

#include "step_pathfinder_game_TerrainData.h"

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
		class TerrainViewerScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			TerrainViewerScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Terrain Viewer"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void resetTerrain();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			
			game::TerrainData mTerrainData;
			game::TerrainViewer* mTerrainViewer4Original;
			game::TerrainViewer* mTerrainViewer4Game;
		};
	}
}
