#pragma once

#include "cocos2d.h"
#include "CPG_Input_AllowedKeys.h"

namespace cpg
{
	namespace input_test
	{
		class KeyboardInputObserver : public cocos2d::Node
		{
		private:
			KeyboardInputObserver();

		public:
			static KeyboardInputObserver* create( const char* _allowed_keys_file_name );

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
			input::AllowedKeys::Container allowed_keys;
			bool found;
		};
	}
}