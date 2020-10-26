#pragma once

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

namespace cpgui
{
	namespace research_layout
	{
		class ToolBarNodeScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		private:
			ToolBarNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "tool bar node"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
