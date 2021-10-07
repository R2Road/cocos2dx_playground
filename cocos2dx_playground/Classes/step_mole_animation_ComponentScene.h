#pragma once

#include <vector>

#include "2d/CCScene.h"

#include "cpg_Animation_Index.h"

#include "helper_BackToThePreviousScene.h"

namespace step_mole
{
	namespace animation
	{
		class ComponentScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			ComponentScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Animation : Component"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

			void PlayAnimation( const cpg_animation::eIndex animation_index );
			void PlayAnimationWithCallback();
			void StopAnimation();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Node* mAnimationNode;
		};
	}
}
