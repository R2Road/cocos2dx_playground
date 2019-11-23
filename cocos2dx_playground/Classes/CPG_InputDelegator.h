#pragma once

#include <vector>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		class Delegator : public cocos2d::Node
		{
		private:
			struct KeyStatus
			{
				KeyStatus() : keycode( cocos2d::EventKeyboard::KeyCode::KEY_NONE ), status( false ) {}
				KeyStatus( cocos2d::EventKeyboard::KeyCode _keycode ) : keycode( _keycode ), status( false ) {}

				cocos2d::EventKeyboard::KeyCode keycode;
				bool status;
			};
			using KeyStatusContainer = std::vector<KeyStatus>;

			Delegator();

		public:
			static Delegator* create();

			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		public:
			const bool getKeyStatus( const cocos2d::EventKeyboard::KeyCode keycode ) const;

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			KeyStatusContainer key_status_container;
		};
	}
}