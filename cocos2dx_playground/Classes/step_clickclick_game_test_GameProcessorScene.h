#pragma once

#include <memory>

#include "2d/CCScene.h"

#include "cpg_GridIndexConverter.h"

namespace step_clickclick
{
	namespace game
	{
		class EffectManagerNode;

		using StageUp = std::unique_ptr<class Stage>;
		class StageViewNode;
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
			void updateScoreView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			game::StageUp mStage;
			game::StageViewNode* mStageViewNode;
			game::EffectManagerNode* mEffectManagerNode;
			const cpg::GridIndexConverter mGridIndexConverter;

			int mScore;
		};
	}
}
