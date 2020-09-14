#pragma once

#include "cocos2d.h"

namespace step_typetype
{
	namespace label_node
	{
		class LabelScene : public cocos2d::Scene
		{
		public:
			LabelScene();

			static const char* getTitle() { return "Label"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
