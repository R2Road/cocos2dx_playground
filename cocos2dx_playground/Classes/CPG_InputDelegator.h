#pragma once

#include "cocos2d.h"
#include "CPG_Input_AllowedKeys.h"
#include "CPG_Input_KeyCodeCollector.h"

namespace cpg
{
	namespace input
	{
		using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;

		class Delegator : public cocos2d::Node
		{
		private:
			Delegator();

		public:
			static Delegator* create( const char* _allowed_keys_file_name );

			void onEnter() override;
			void update( float _dt ) override;
			void post_update( float _dt );
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		public:
			inline const bool isActiveKey( const cocos2d::EventKeyboard::KeyCode _keycode ) const { return keycode_collector.isActiveKey(_keycode ); }
			void addInputCollector( KeyCollectorSp& _new_key_collector );

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			AllowedKeys::Container allowed_keys;
			KeyCodeCollector keycode_collector;
			KeyCollectorSp input_collector;
		};
	}
}