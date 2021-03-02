#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace step_mole
{
	namespace animation
	{
		class CallbackScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			CallbackScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Animation : Callback"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

			void AnimationStartCallback();
			void AnimationEndCallback();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Node* mAnimationNode;
			cocos2d::Action* mSequenceAction;
		};
	}
}
