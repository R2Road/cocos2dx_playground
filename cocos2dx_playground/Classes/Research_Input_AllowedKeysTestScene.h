#pragma once

#include <string>

#include "cocos2d.h"
#include "ui/UIWidget.h"

namespace cpg
{
	namespace Input
	{
		class Delegator;
	}

	namespace InputTest
	{
		class KeyboardInputObserver;
	}
}

namespace Research
{
	namespace Input
	{
		class AllowedKeysTestScene : public cocos2d::Scene
		{
		public:
			AllowedKeysTestScene();

			static cocos2d::Scene* create();

			bool init() override;
			void update( float dt ) override;

		private:
			void onExitButton( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );
			void update_forExit( float dt );

		private:
			cpg::Input::Delegator* input_delegator;
			cocos2d::Label* key_viewer;
			std::string key_string;

			cpg::InputTest::KeyboardInputObserver* input_observer;
		};
	}
}
