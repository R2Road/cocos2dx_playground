#pragma once

#include "cocos2d.h"

namespace step01
{
	namespace game
	{
		class MapToolScene : public cocos2d::Scene
		{
		private:
			MapToolScene();

		public:
			static const char* getTitle() { return "Game : Map Tool"; }
			static cocos2d::Scene* create();

			bool init() override;
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
