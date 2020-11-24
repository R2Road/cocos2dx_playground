#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace ui_research
{
	namespace type_effect
	{
		class ShakeScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			ShakeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "TypeEffect : Shake"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void HideLetters();
			void StartType();
			void EndType();
			void updateForType( float dt );

			void updateDelayView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			float mTypeDelay;
			float mElapsedTime;
			int mLetterIndicator;
		};
	}
}
