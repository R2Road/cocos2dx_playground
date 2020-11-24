#pragma once

#include "2d/CCScene.h"
#include "math/CCGeometry.h"
#include "ui/UIWidget.h"
#include "ui/UILayoutParameter.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
namespace ui
{
	class Layout;
}
NS_CC_END

namespace ui_practice
{
	namespace layout_test
	{
		class VariousItemSizeScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			VariousItemSizeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Layout - Various Item Size"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			cocos2d::ui::Layout* MakeLayoutVertical(
				const int button_visible_count
				, const cocos2d::Size button_size
				, const cocos2d::Size button_margin
				, const bool clipping_enable
			);
			cocos2d::Node* MakeLayoutItem(
				const int item_tag
				, const cocos2d::Size button_size
				, const cocos2d::Size button_margin
				, const cocos2d::ui::LinearLayoutParameter::LinearGravity horizontal_gravity
			);

			void onDummyButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchEventType );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::ui::Layout* mLayout;
			bool mDirection;
		};
	}
}
