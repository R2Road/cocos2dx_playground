#pragma once

#include "2d/CCScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollector.h"
#include "step_rain_of_chaos_game_StageConfig.h"

namespace step_rain_of_chaos
{
	namespace game
	{
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

			void UpdateForInput( float delta_time );

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;

			game::StageConfig mStageConfig;
			game::StageNode* mStageNode;
		};
	}
}
