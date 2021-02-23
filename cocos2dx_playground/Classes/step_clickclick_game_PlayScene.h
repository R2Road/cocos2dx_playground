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
			enum eStep
			{
				wait_for_entry,
				show_clear_indicator,
				wait_for_count,
				hide_clear_indicator,
				reset,
				game_clear,
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

			int mStep = eStep::wait_for_entry;
			float mElapsedTime = 0.f;
		};
	}
}
