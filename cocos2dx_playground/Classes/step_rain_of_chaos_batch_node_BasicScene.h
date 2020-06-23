#pragma once

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

namespace step_rain_of_chaos
{
	namespace batch_node
	{
		class BasicScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		private:
			BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Batch Node : Basic"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateLifeTimeView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
