#pragma once

#include "cocos2d.h"

namespace step_typetype
{
	namespace sprite
	{
		class AnchorPointScene : public cocos2d::Scene
		{
		public:
			AnchorPointScene();

			static const char* getTitle() { return "Sprite : AnchorPoint"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			int mPressedKeyCount;
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
