#pragma once

#include "cocos2d.h"
#include "CPG_Input_AllowedKeys.h"

namespace Research
{
	namespace Input
	{
		class KeyAllowScene : public cocos2d::Scene
		{
		public:
			KeyAllowScene();

			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void update( float _dt ) override;
			void onExit() override;

		private:
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			bool go_exit;
			CPG::Input::AllowedKeys allowed_keys;
		};
	}
}
