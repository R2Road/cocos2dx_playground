#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	namespace game
	{
		class IndicatorViewNode;
	}

	namespace game_test
	{
		class IndicatorViewNodeScene : public cocos2d::Scene	
		{
		private:
			IndicatorViewNodeScene();

		public:
			static const char* getTitle() { return "Game Test : Indicator View Node"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void resetIndicatorView();
			void updateStageLengthView();
			void updateIndicatorPositionView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			std::size_t mCurrentStageLength;
			int mCurrentIndicatorPosition;
			game::IndicatorViewNode* mIndicatorViewNode;
		};
	}
}
