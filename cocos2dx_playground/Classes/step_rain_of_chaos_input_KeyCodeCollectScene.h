#pragma once

#include <string>

#include "2d/CCScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollector.h"

NS_CC_BEGIN
class Label;
NS_CC_END

namespace step_rain_of_chaos
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

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void updateKeyViewer();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;

			std::string mKeyStrings;
			cocos2d::Label* mKeyViewer;
		};
	}
}
