#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN;
	class Sprite;
NS_CC_END;

namespace step_flipflip
{
	namespace sprite_frame
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Sprite Frame : Basic"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Sprite* mTestNode;
		};
	}
}
