#pragma once

#include <memory>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

namespace step_mole
{
	namespace game
	{
		using TargetManagerUp = std::unique_ptr<class TargetManager>;
		class StageNode;
	}

	namespace game_test
	{
		class SpawnAndHitScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			SpawnAndHitScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Spawn And Hit"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForSpawnProcessStart( const float dt );
			void updateForSpawn( const float dt );
			void attackProcess( const int world_x, const int world_y );

			void updateSpawnTargetCountView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::TargetManagerUp mTargetManager;
			game::StageNode* mStageNode;

			int mCurrentSpawnTargetCount;
		};
	}
}
