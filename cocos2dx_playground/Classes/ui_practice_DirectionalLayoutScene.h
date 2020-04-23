#pragma once

#include "2d/CCScene.h"
#include "math/CCGeometry.h"
#include "ui/UIWidget.h"

namespace ui_practice
{
	class DirectionalLayoutScene : public cocos2d::Scene
	{
	public:
		DirectionalLayoutScene();

		static const char* getTitle() { return "Directional Layout"; }
		static cocos2d::Scene* create();

		bool init() override;
		void onEnter() override;
		void onExit() override;

	private:
		Node* MakeLayoutVertical(
			const int button_visible_count
			, const int button_count
			, const cocos2d::Size button_size
			, const cocos2d::Size button_margin
			, const bool clipping_enable
		);

		Node* MakeLayoutHorizontal(
			const int button_visible_count
			, const int button_count
			, const cocos2d::Size button_size
			, const cocos2d::Size button_margin
			, const bool clipping_enable
		);

		void onDummyButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchEventType );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
