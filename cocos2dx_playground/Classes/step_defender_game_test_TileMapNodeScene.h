#pragma once

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}

	namespace game_test
	{
		class TileMapNodeScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			TileMapNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : TileMap"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_defender::game::TileMapNode* mTileMapNode;
		};
	}
}
