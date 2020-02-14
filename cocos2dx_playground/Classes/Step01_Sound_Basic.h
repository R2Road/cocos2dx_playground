#pragma once

#include "cocos2d.h"

namespace step01
{
	namespace sound
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			~BasicScene();

			static const char* getTitle() { return "Sound : Basic"; }
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
