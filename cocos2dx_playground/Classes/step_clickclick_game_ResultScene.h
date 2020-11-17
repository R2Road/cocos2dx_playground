#pragma once

#include "2d/CCScene.h"

namespace step_clickclick
{
	namespace game
	{
		class ResultScene : public cocos2d::Scene
		{
		private:
			ResultScene();

		public:
			static const char* getTitle() { return "Game : Path Finder - Result"; }
			static cocos2d::Scene* create( const int clear_score );

		private:
			bool init( const int clear_score );

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
