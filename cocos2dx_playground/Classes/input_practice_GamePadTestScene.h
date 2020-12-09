#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
	class EventListenerController;
NS_CC_END

namespace input_practice
{
	class GamePadTestScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		GamePadTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "GamePad Test"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void updateKeyCodeView( const int keycode );
		void clearKeyCodeView();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		cocos2d::EventListenerController* mControllerListener;

		int mPressedKeyCount;
	};
}
