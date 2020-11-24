#pragma once

#include "2d/CCScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollector.h"
#include "step_rain_of_chaos_game_EnemyNode.h"
#include "step_rain_of_chaos_game_StageConfig.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageNode;

		class PlayScene : public cocos2d::Scene
		{
		private:
			enum eIntroStep
			{
				FadeIn,
				FadeInWait,

				FadeInPlayer,
				FadeInPlayerSound,
				FadeInPlayerWait,

				FadeInEnemy,
				FadeInEnemySound,
				FadeInEnemyWait,

				EnemyProcessStart,

				Ready,
				ReadyWait_1,
				ReadyWait_2,

				Go,
				GoWait_1,
				GoWait_2,

				StartGame,

				Test,
			};

			enum eGameOverStep
			{
				DyingMessage,
				FadeInGameOver,

				Exit = FadeInGameOver + 2,
			};

			using PackageContainer = std::vector<game::EnemyNode::EnemyProcessorContainer>;

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
			void update4Intro( float delta_time );
			void update4Game( float delta_time );
			void update4GameOver( float delta_time );

			void onEnemyProcessEnd();
			void startEnemyProcess();
			void playerHasDamage();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

			void buildProcessor();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;
			int mAudioID_forBGM;

			game::StageConfig mStageConfig;
			game::StageNode* mStageNode;
			cocos2d::Node* mBackgroundNode;

			int mStep;
			PackageContainer mPackgeContainer;
			std::size_t mPackageIndicator;
			std::size_t mPackageIndicatorWhenPlayerDie;
		};
	}
}
