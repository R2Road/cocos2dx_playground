#pragma once

#include <vector>

#include "cocos2d.h"

#include "cpg_Animation_Index.h"

namespace step_rain_of_chaos
{
	namespace animation
	{
		class ComponentScene : public cocos2d::Scene
		{
		private:
			ComponentScene();

		public:
			static const char* getTitle() { return "Animation : Component"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			void PlayAnimation( const cpg::animation::eIndex animation_index );
			void StopAnimation();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
