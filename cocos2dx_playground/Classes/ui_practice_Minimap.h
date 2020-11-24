#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace ui_practice
{
	class Minimap : public cocos2d::Scene
	{
	private:
		Minimap();

	public:
		static const char* getTitle() { return "Minimap"; }
		static cocos2d::Scene* create();

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;
		void update( float dt ) override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
		void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		void updateMoveSpeedLabel();

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		int mCurrentPressedCount;
		cocos2d::Vec2 mCameraMoveVec2;
		int mCameraMoveSpeed;
	};
}
