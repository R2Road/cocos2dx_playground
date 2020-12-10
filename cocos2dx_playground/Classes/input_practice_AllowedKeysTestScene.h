#pragma once

#include <string>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

NS_CC_BEGIN
class Label;
NS_CC_END

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

namespace input_practice
{
	class AllowedKeysTestScene : public cocos2d::Scene
	{
	private:
		AllowedKeysTestScene();

	public:
		static cocos2d::Scene* create();

	private:
		bool init() override;

	public:
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
