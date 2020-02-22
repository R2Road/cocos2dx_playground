#pragma once

#include <string>

#include "cocos2d.h"

#include "CPG_Input_KeyCodeCollector.h"

namespace step02
{
	namespace input
	{
		class KeyCodeCollectScene : public cocos2d::Scene
		{
		private:
			KeyCodeCollectScene();

		public:
			static const char* getTitle() { return "Input : KeyCode Collect"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );
			void updateKeyViewer();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cpg::input::KeyCodeCollector mKeyCodeCollector;

			std::string mKeyStrings;
			cocos2d::Label* mKeyViewer;
		};
	}
}
