#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	namespace game
	{
		using TargetManagerUp = std::unique_ptr<class TargetManager>;
		class StageNode;

		class ProcessAction
		{
		public:
			using ActionFunc = std::function<void(float, int)>;

			ProcessAction( const float life_time, const int spawn_count, const float delay_time, const int repeat_count, const ActionFunc& action_func );
			ProcessAction( const float delay_time, const ActionFunc& action_func );

			bool Update( float dt );

		private:
			float mLifeTime;

			int mRepeatLimit;
			int mCurrentRepeatCount;

			int mSpawnCount;

			float mDelayTime;
			float mElapsedTime;

			ActionFunc mActionFunc;
		};

		class PlayScene : public cocos2d::Scene
		{
		private:
			using ProcessActionContainerT = std::vector<ProcessAction>;

			PlayScene();

		public:
			static const char* getTitle() { return "Game : Play"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

			void update4Game( float dt );

		private:
			void doSpawn( const float life_time, const int spawn_count );
			void attackProcess( const int world_x, const int world_y );

			void updateScoreView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			int mAudioID_forBGM;

			TargetManagerUp mTargetManager;
			StageNode* mStageView;

			int mCurrentSpawnTargetCount;

			ProcessActionContainerT mProcessActionContainer;
			ProcessActionContainerT::iterator mProcessActionIndicator;

			int mScore;
		};
	}
}
