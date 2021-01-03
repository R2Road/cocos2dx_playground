#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	namespace game
	{
		class StageViewNode;
	}

	namespace game_test
	{
		class StageViewNodeScene : public cocos2d::Scene	
		{
		private:
			StageViewNodeScene();

		public:
			static const char* getTitle() { return "Game Test : Stage View Node"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void resetStage();
			void updateStageLengthView();
			void updateIndicatorPositionView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			std::size_t mCurrentStageLength;
			int mCurrentIndicatorPosition;
			game::StageViewNode* mStageViewNode;
		};
	}
}
