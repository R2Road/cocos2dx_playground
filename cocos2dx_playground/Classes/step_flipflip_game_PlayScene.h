#pragma once

#include "2d/CCScene.h"

#include "cpg_Point.h"

#include "step_flipflip_game_Constant.h"
#include "step_flipflip_game_StageData.h"

namespace step_flipflip
{
	namespace game
	{
		class CardSelectorNode;
		class MessageViewNode;
		class StageViewNode;

		class PlayScene : public cocos2d::Scene
		{
		private:
			enum eStep
			{
				Enter,

				Message4Hint,
				Sleep4Message4Hint,
				ShowHint,
				Sleep4ShowHint,
				HideHint,
				Sleep4HideHint,

				Wait4Message4Game,
				Message4Game,
				Sleep4Message4Game,
				BGM_Start,

				Game_ShowIndicator,
				Game_SelectCard,
				Game_HideIndicator,
				Game_Wait4DecideCard,
				Game_DecideCard,
				Game_SelectFailed,
				Game_SelectSuccess,
				Game_ClearCheck,
				Game_Result,
			};

			PlayScene();

		public:
			static const char* getTitle() { return "Game : Play"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void update4Game( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			int mAudioID_forBGM;

			StageConfig mStageConfig;
			StageData mStageData;
			CardSelectorNode* mCardSelectorNode;
			MessageViewNode* mMessageViewNode;
			StageViewNode* mStageViewNode;

			int mStep;
			float mElapsedTime;
			int mFlipedCount;
			cpg::Point mFlipedPoints[2];

			float mPlayTime;
			int mFailedCount;
		};
	}
}
