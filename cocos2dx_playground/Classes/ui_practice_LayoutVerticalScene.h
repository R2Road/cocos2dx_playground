#pragma once

#include "2d/CCScene.h"
#include "math/CCGeometry.h"
#include "ui/UIWidget.h"
#include "ui/UILayoutParameter.h"

#include "helper_BackToThePreviousScene.h"

namespace ui_practice
{
	class LayoutVerticalScene : public cocos2d::Scene, public helper::BackToThePreviousScene
	{
	public:
		LayoutVerticalScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		static const char* getTitle() { return "Layout - Vertical"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		bool init() override;
		void onEnter() override;
		void onExit() override;

	private:
		Node* MakeLayoutVertical(
			const int button_visible_count
			, const int button_content_count
			, const cocos2d::Size button_size
			, const cocos2d::Size button_margin
			, const cocos2d::ui::LinearLayoutParameter::LinearGravity horizontal_gravity
			, const bool clipping_enable
		);

		void onDummyButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchEventType );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
