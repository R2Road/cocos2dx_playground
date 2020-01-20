#pragma once

#include "cocos2d.h"

namespace research
{
	namespace input
	{
		class BasicScene : public cocos2d::Scene
		{
		public:
			BasicScene();

			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void updateKeyCodeView( cocos2d::EventKeyboard::KeyCode keycode );
			void clearKeyCodeView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			int mPressedKeyCount;
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
