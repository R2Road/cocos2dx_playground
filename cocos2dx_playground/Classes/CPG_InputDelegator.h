#pragma once

#include "cocos2d.h"
#include "CPG_Input_AllowedKeys.h"
#include "cpg_input_KeyCodeCollector.h"

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
			static Delegator* create( const char* allowed_keys_file_name );

			void onEnter() override;
			void update( float dt ) override;
			void post_update( float dt );
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		public:
			inline const bool isActiveKey( const cocos2d::EventKeyboard::KeyCode keycode ) const { return mKeycodeCollector.isActiveKey( keycode ); }
			void addInputCollector( KeyCollectorSp& new_key_collector );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			AllowedKeys::Container mAllowedKeys;
			step_rain_of_chaos::input::KeyCodeCollector mKeycodeCollector;
			KeyCollectorSp mInputCollector;
		};
	}
}