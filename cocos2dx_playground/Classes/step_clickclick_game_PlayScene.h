#pragma once

#include <memory>

#include "2d/CCScene.h"

namespace step_flipflip
{
	namespace game
	{
		class MessageViewNode;
	}
}

namespace step_clickclick
{
	namespace game
	{
		class EffectManagerNode;

		using StageUp = std::unique_ptr<class Stage>;
		class StageViewNode;

		class PlayScene : public cocos2d::Scene
		{
		private:
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
			void onGameProcess( const int block_linear_index );
			void updateScoreView();
			void updateForNextStep( float dt );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			int mAudioID_forBGM;

			StageUp mStage;
			StageViewNode* mStageViewNode;
			EffectManagerNode* mEffectManagerNode;
			step_flipflip::game::MessageViewNode* mMessageViewNode;

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
			};
			NextStepData mNextStepData;
		};
	}
}
