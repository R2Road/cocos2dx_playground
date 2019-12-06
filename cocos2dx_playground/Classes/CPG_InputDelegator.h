#pragma once

#include <vector>

#include "cocos2d.h"
#include "CPG_Input_AllowedKeys.h"

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
			static Delegator* create();

			void onEnter() override;
			void update( float _dt ) override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		public:
			void addInputCollector( BasicCollectorSp& _new_input_collector );

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			AllowedKeys allowed_keys;
			BasicCollectorSp input_collector;
		};
	}
}