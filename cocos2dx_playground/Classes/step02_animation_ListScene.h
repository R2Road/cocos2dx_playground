#pragma once

#include <vector>

#include "cocos2d.h"

#include "cpg_Animation_Index.h"

namespace step02
{
	namespace animation
	{
		class ListScene : public cocos2d::Scene
		{
		private:
			ListScene();

		public:
			static const char* getTitle() { return "Animation : List"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			void playAnimation( const cpg::animation::eIndex animation_index );
			void stopAnimation();
			cocos2d::Action* getAnimationAction( const cpg::animation::eIndex animation_index );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			std::vector<cocos2d::Action*> mAnimationActions;
		};
	}
}