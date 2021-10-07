#pragma once

#include <vector>

#include "2d/CCScene.h"

#include "cpg_Animation_Index.h"

namespace step_clickclick
{
	namespace animation
	{
		class ListScene : public cocos2d::Scene
		{
		private:
			ListScene();

		public:
			~ListScene();

			static const char* getTitle() { return "Animation : List"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void playAnimation( const cpg_animation::eIndex animation_index );
			cocos2d::Action* getAnimationAction( const cpg_animation::eIndex animation_index ) const;

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Node* mAnimationNode;
			std::vector<cocos2d::Action*> mAnimationActions;
		};
	}
}
