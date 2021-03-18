#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

#include "cpg_input_AllowedKeys.h"
#include "cpg_input_KeyMapConfigHelper.h"

namespace input_practice
{
	class KeyConfigScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		KeyConfigScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Key Config"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyConfigControl( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		void onExitButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cpg_input::AllowedKeys::Container mAllowedKeys;
		cpg_input::KeyMapConfigHelper mKeymapConfigHelper;
		cocos2d::Node* mCurrentButtonNode;
	};
}
