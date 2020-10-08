#pragma once

#include "2d/CCScene.h"

#include "step_typetype_game_Stage.h"

namespace step_typetype
{
	namespace game
	{
		class StageView;

		class PlayScene : public cocos2d::Scene	
		{
		private:
			PlayScene();

		public:
			static const char* getTitle() { return "Game : Play"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void update( float dt ) override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			std::size_t mCurrentStageLength;
			Stage mStage;
			StageView* mStageView;
			double mElapsedTime;
			int mAudioID_forBGM;
		};
	}
}
