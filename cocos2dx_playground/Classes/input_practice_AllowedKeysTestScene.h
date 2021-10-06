#pragma once

#include <string>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace cpg_input
{
	class DelegatorNode;
	class KeyboardInputObserverNode;
}

namespace input_practice
{
	class AllowedKeysTestScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		AllowedKeysTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Allowed Keys Test"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void update( float dt ) override;

	private:
		void onExitButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

	private:
		cpg_input::DelegatorNode* mInputDelegatorNode;
		cocos2d::Label* mKeyViewer;
		std::string mKeyStrings;

		cpg_input::KeyboardInputObserverNode* mKeyboardInputObserverNode;
	};
}
