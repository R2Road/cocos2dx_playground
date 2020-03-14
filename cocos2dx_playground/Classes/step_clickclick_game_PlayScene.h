#pragma once

#include <memory>

#include "2d/CCScene.h"

namespace step_clickclick
{
	namespace game
	{
		using StageUp = std::unique_ptr<class Stage>;
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
			void onExit() override;

		private:
			void onGameProcess( const int block_linear_index );
			void updateScoreView();
			void updateCountView( const float count );
			void updateForNextStep( float dt );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			StageUp mStage;
			StageView* mStageView;

			int mScore;
			int mCurrentStageWidth;
			int mCurrentStageHeight;

			struct NextStepData
			{
				enum eStep
				{
					wait_for_entry,
					show_clear_indicator,
					wait_for_count,
					hide_clear_indicator,
					reset,
					game_clear,
				};

				int Step = eStep::wait_for_entry;
				float ElapsedTime_forEntry = 0.f;
				const float LimitTime_forEntry = 0.6f;
				float ElapsedTime_forCount = 0.f;
				const float LimitTime_forCount = 3.f;
			};
			NextStepData mNextStepData;
		};
	}
}
