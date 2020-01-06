#pragma once

#include <string>

#include "cocos2d.h"
#include "ui/UIWidget.h"

namespace cpg
{
	namespace input
	{
		class Delegator;
	}

	namespace input_test
	{
		class KeyboardInputObserver;
	}
}

namespace research
{
	namespace input
	{
		class AllowedKeysTestScene : public cocos2d::Scene
		{
		public:
			AllowedKeysTestScene();

			static cocos2d::Scene* create();

			bool init() override;
			void update( float dt ) override;

		private:
			void onExitButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void update_forExit( float dt );

		private:
			cpg::input::Delegator* mInputDelegator;
			cocos2d::Label* mKeyViewer;
			std::string mKeyStrings;

			cpg::input_test::KeyboardInputObserver* mInputObserver;
		};
	}
}
