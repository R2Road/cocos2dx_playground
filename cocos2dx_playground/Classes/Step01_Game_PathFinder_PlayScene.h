#pragma once

#include "cocos2d.h"

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			class PlayScene : public cocos2d::Scene
			{
			private:
				PlayScene();

			public:
				static const char* getTitle() { return "Game : Path Finder - Play Scene"; }
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
