#pragma once

#include <memory>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	namespace game
	{
		using TargetManagerUp = std::unique_ptr<class TargetManager>;
		class StageNode;

		class PlayScene : public cocos2d::Scene
		{
		private:
			PlayScene();

		public:
			static const char* getTitle() { return "Game : Play"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForSpawnProcessStart( const float dt );
			void updateForSpawn( const float dt );
			void attackProcess( const int world_x, const int world_y );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			int mAudioID_forBGM;

			TargetManagerUp mTargetManager;
			StageNode* mStageView;

			int mCurrentSpawnTargetCount;
		};
	}
}
