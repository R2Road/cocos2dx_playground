#pragma once

#include "cocos2d.h"

namespace CPG
{
	namespace InputTest
	{
		class KeyboardInputObserber : public cocos2d::Node
		{
		private:
			KeyboardInputObserber();

		public:
			static KeyboardInputObserber* create();

			void onEnter() override;
			void update( float _dt ) override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		public:
			inline const bool inputFound() const { return found; }

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			bool found;
		};
	}
}