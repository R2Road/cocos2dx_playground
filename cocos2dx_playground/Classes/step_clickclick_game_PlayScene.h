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
				Enter,

				ShowLevelIndicator,
				Wait4LevelIndicator,

				StartGame,
				PlayGame,

				StageClear,

				ShowClearIndicator,
				Wait4ClearIndicator,

				GameClear,
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

			int mStep = eStep::StageClear;
			float mElapsedTime = 0.f;
		};
	}
}
