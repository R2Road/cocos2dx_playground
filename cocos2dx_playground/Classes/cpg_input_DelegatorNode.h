#pragma once

#include "2d/CCNode.h"

#include "cpg_input_AllowedKeys.h"
#include "step_rain_of_chaos_input_KeyCodeCollector.h"

namespace cpg_input
{
	using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;

	class DelegatorNode : public cocos2d::Node
	{
	private:
		DelegatorNode();

	public:
		static DelegatorNode* create( const char* allowed_keys_file_name );

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
		inline bool isActiveKey( const cocos2d::EventKeyboard::KeyCode keycode ) const { return mKeycodeCollector.isActiveKey( keycode ); }
		void addInputCollector( KeyCollectorSp& new_key_collector );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		AllowedKeys::Container mAllowedKeys;
		step_rain_of_chaos::input::KeyCodeCollector mKeycodeCollector;
		KeyCollectorSp mInputCollector;
	};
}