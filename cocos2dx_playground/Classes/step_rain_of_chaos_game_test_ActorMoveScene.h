#pragma once

#include <string>

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"
#include "step_rain_of_chaos_input_KeyCodeCollector.h"

NS_CC_BEGIN
class Label;
NS_CC_END

namespace step_rain_of_chaos
{
	namespace game_test
	{
		class ActorMoveScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		private:
			ActorMoveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Game Test : Actor Move"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
			void onEnter() override;
			void onExit() override;
			void update( float dt ) override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;
		};
	}
}
