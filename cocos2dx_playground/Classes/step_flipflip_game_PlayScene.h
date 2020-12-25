#pragma once

#include "2d/CCScene.h"

namespace step_flipflip
{
	namespace game
	{
		class CardSelectorNode;
		class StageViewNode;

		class PlayScene : public cocos2d::Scene
		{
		private:
			enum eStep
			{
				Enter,
				ShowHint,
				Sleep4ShowHint,
				HideHint,
				Sleep4HideHint,
				Game_Start,
				Game,
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

			void Update4GameStart( float dt );

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			int mAudioID_forBGM;

			CardSelectorNode* mCardSelectorNode;
			StageViewNode* mStageViewNode;

			int mStep;
			float mElapsedTime;
			bool mbInputEnable;
		};
	}
}
