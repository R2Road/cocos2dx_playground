#pragma once

#include <vector>

#include "cocos2d.h"

namespace research
{
	namespace animation
	{
		class ListScene : public cocos2d::Scene
		{
		private:
			enum class eAnimationIndex
			{
				none = 0,
				run,
				idle,
				win,
			};

			ListScene();

		public:
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			cocos2d::Action* getAnimationAction( const eAnimationIndex animation_index );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			std::vector<cocos2d::Action*> mAnimationActions;
		};
	}
}
