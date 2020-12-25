#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_flipflip
{
	namespace game
	{
		class StageViewNode;
	}

	namespace game_test
	{
		class StageViewScene : public cocos2d::Scene
		{
		private:
			StageViewScene();

		public:
			static const char* getTitle() { return "Game Test : Stage View"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void buildStageView();
			void updateShuffleCountView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::StageViewNode* mStageViewNode;
			int mShuffleCount;
			cocos2d::Label* mShuffleCountLabel;
		};
	}
}
