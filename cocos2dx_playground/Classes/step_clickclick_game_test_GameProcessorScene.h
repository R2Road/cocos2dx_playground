#pragma once

#include <memory>

#include "2d/CCScene.h"

#include "cpg_GridIndexConverter.h"

namespace step_clickclick
{
	namespace game
	{
		using StageUp = std::unique_ptr<class Stage>;
		class StageView;
	}

	namespace game_test
	{
		class GameProcessorScene : public cocos2d::Scene
		{
		private:
			GameProcessorScene();

		public:
			static const char* getTitle() { return "Game Test : Game Processor"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onGameProcess( const int block_linear_index );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

			void updateScoreView();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			game::StageUp mStage;
			game::StageView* mStageView;
			const cpg::GridIndexConverter mGridIndexConverter;

			int mScore;
		};
	}
}
