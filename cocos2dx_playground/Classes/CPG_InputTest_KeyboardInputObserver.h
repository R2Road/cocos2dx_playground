#pragma once

#include "2d/CCNode.h"

#include "cpg_input_AllowedKeys.h"

namespace cpg_input
{
	class KeyboardInputObserver : public cocos2d::Node
	{
	private:
		KeyboardInputObserver();

	public:
		static KeyboardInputObserver* create( const char* allowed_keys_file_name );

	private:
		bool init( const char* allowed_keys_file_name );

	public:
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
		cpg_input::AllowedKeys::Container mAllowedKeys;
		bool mbFound;
	};
}