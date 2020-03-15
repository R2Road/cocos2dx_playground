#pragma once

#include "2d/CCScene.h"

#include "step_typetype_game_Stage.h"

namespace step_typetype
{
	namespace game
	{
		class StageTestScene : public cocos2d::Scene
		{
		private:
			StageTestScene();

		public:
			static const char* getTitle() { return "Game : Stage Test"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateStage();

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			std::size_t mCurrentStageLength;
			Stage mStage;
		};
	}
}
