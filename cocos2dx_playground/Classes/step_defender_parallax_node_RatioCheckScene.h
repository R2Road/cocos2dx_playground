#pragma once

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollector.h"

namespace step_defender
{
	namespace parallax_node
	{
		class RatioCheckScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			RatioCheckScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Parallax Node : Ratio Check"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

			void update4Move( float delta_time );

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;
		};
	}
}
