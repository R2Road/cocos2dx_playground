#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

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
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			int mPressedKeyCount;
			cocos2d::Label* mKeyVodeView;
		};
	}
}
