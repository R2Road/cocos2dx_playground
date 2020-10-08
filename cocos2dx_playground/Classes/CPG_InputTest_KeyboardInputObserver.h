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
			static KeyboardInputObserver* create( const char* allowed_keys_file_name );

			void onEnter() override;
			void update( float dt ) override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		public:
			inline const bool inputFound() const { return mbFound; }

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			input::AllowedKeys::Container mAllowedKeys;
			bool mbFound;
		};
	}
}