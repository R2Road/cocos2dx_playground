#pragma once

#include <vector>

#include "cocos2d.h"
#include "CPG_Input_AllowedKeys.h"
#include "CPG_Input_KeyCodeCollector.h"

namespace CPG
{
	namespace Input
	{
		using BasicCollectorSp = std::shared_ptr<class BasicCollector>;

		class Delegator : public cocos2d::Node
		{
		private:
			Delegator();

		public:
			static Delegator* create( const char* _allowed_keys_file_name );

			void onEnter() override;
			void update( float _dt ) override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		public:
			inline const bool hasChanged() const { return keycode_collector.hasChanged(); }
			inline const bool isActiveKey( const cocos2d::EventKeyboard::KeyCode _keycode ) const { return keycode_collector.isActiveKey(_keycode ); }
			void addInputCollector( BasicCollectorSp& _new_input_collector );

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			AllowedKeys allowed_keys;
			KeyCodeCollector keycode_collector;
			BasicCollectorSp input_collector;
		};
	}
}