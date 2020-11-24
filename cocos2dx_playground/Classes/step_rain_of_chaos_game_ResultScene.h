#pragma once

#include "2d/CCScene.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class ResultScene : public cocos2d::Scene
		{
		private:
			ResultScene();

		public:
			static const char* getTitle() { return "Game : Result"; }
			static cocos2d::Scene* create( const int total_wave_count, const int clear_wave_count );

		private:
			bool init( const int total_wave_count, const int clear_wave_count );

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
