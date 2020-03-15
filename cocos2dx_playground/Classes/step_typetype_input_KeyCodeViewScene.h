#pragma once

#include "cocos2d.h"

namespace step_typetype
{
	namespace input
	{
		class KeyCodeViewScene : public cocos2d::Scene
		{
		public:
			KeyCodeViewScene();

			static const char* getTitle() { return "Input : Keycode View"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
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
