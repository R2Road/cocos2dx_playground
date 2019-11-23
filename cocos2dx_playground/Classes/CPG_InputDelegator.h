#pragma once

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		class Delegator : public cocos2d::Node
		{
		private:
			Delegator();

		public:
			static Delegator* create();

			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		public:
			const bool keyPressed_ESC() const { return key_pressed_esc; }

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;

			bool key_pressed_esc;
		};
	}
}