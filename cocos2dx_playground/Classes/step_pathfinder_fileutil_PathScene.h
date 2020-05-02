#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace step_pathfinder
{
	namespace fileutil
	{
		class PathScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		public:
			PathScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			static const char* getTitle() { return "Path"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
