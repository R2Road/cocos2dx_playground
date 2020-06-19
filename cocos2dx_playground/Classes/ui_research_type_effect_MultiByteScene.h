#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace ui_research
{
	namespace type_effect
	{
		class MultiByteScene : public cocos2d::Scene, public helper::BackToThePreviousScene
		{
		private:
			MultiByteScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "TypeEffect : Multi Byte"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			bool init() override;
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
