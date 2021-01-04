#pragma once

#include "2d/CCScene.h"

#include "step_typetype_game_Stage.h"

namespace step_typetype
{
	namespace game
	{
		class IndicatorViewNode;
		class StageViewNode;
	}

	namespace game_test
	{
		class GameProcessScene : public cocos2d::Scene	
		{
		private:
			GameProcessScene();

		public:
			static const char* getTitle() { return "Game Test : Game Process"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateStageLengthView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			std::size_t mCurrentStageLength;
			game::Stage mStage;
			game::StageViewNode* mStageViewNode;
			game::IndicatorViewNode* mIndicatorViewNode;
		};
	}
}
