#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN;
	class Label;
NS_CC_END;

namespace step_flipflip
{
	namespace scale
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Scale : Basic"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateScaleView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Node* mTestNode;
			cocos2d::Label* mScaleView;
		};
	}
}
