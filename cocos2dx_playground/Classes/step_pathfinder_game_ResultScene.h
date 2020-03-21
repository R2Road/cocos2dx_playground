#pragma once

#include "cocos2d.h"

namespace step_pathfinder
{
	namespace game
	{
		class ResultScene : public cocos2d::Scene
		{
		private:
			ResultScene();

		public:
			static const char* getTitle() { return "Game : Path Finder - Result"; }
			static cocos2d::Scene* create( const float clear_time );

			bool init( const float clear_time );
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
