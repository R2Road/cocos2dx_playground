#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"
#include "step_rain_of_chaos_game_StageConfig.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageNode;
	}

	namespace game_test
	{
		class StageNodeScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		private:
			StageNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Stage Node"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::StageConfig mStageConfig;
			game::StageNode* mStageNode;
		};
	}
}
