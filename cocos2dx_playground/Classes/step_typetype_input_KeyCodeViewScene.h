#pragma once

#include "2d/CCScene.h"

namespace step_typetype
{
	namespace input
	{
		class KeyCodeViewScene : public cocos2d::Scene
		{
		private:
			KeyCodeViewScene();

		public:
			static const char* getTitle() { return "Input : Keycode View"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateKeyCodeView( cocos2d::EventKeyboard::KeyCode keycode );
			void clearKeyCodeView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			int mPressedKeyCount;
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
