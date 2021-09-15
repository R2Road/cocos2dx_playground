#pragma once

#include "2d/CCScene.h"

namespace ui_research
{
	namespace gauge_research
	{
		class RootScene : public cocos2d::Scene
		{
		private:
			RootScene();

		public:
			static const char* getTitle() { return "Gauge Research"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* key_event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
