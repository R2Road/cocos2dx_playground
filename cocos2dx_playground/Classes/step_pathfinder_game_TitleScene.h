#pragma once

#include "cocos2d.h"

namespace step_pathfinder
{
	namespace game
	{
		namespace pathfinder
		{
			class TitleScene : public cocos2d::Scene
			{
			private:
				TitleScene();

			public:
				static const char* getTitle() { return "Game : Path Finder - Title"; }
				static cocos2d::Scene* create();

				bool init() override;
				void onEnter() override;
				void onExit() override;

			private:
				void updateForExit( float dt );
				void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			private:
				cocos2d::EventListenerKeyboard* mKeyboardListener;
			};
		} // namespace pathfinder
	}
}
