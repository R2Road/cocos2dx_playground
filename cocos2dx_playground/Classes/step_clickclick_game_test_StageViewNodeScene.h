#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_clickclick
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
			void updateShuffleCountView();
			void updateStageSizeView();
			void onBlockSelected( const int block_linear_index );

			void stageSetup();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::StageViewNode* mStageViewNode;
			cocos2d::Label* mShuffleCountLabel;
			cocos2d::Label* mStageSizeLabel;
			cocos2d::Label* mSelectedBlockIndexLabel;
			int mShuffleCount;
			int mCurrentStageWidth;
			int mCurrentStageHeight;
		};
	}
}
