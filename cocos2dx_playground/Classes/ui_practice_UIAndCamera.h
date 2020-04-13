#pragma once

#include <bitset>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace ui_practice
{
	class UIAndCamera : public cocos2d::Scene
	{
	private:
		enum class eArrow
		{
			Up
			, Down
			, Right
			, Left

			, SIZE
			, FIRST = Up,
		};
		UIAndCamera();

	public:
		static const char* getTitle() { return "UI And Camera"; }
		static cocos2d::Scene* create();

		bool init() override;
		void onEnter() override;
		void onExit() override;
		void update( float dt ) override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
		void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		void setArrowFlag( const eArrow target_arrow, const bool bStatus );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		std::bitset<4u> mArrowFlags;
	};
}
