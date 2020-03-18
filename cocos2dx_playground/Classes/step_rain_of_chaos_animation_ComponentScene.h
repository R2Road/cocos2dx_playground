#pragma once

#include <vector>

#include "2d/CCScene.h"

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
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			void PlayAnimation( const cpg::animation::eIndex animation_index );
			void StopAnimation();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
