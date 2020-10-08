#pragma once

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

namespace step_rain_of_chaos
{
	namespace draw_node
	{
		class CircleScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			CircleScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Draw Node : Circle"; }
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
