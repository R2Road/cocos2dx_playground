#pragma once

#include "2d/CCScene.h"

namespace step02
{
	namespace game
	{
		namespace rain_of_chaos
		{
			class TitleScene : public cocos2d::Scene
			{
			private:
				TitleScene();

			public:
				static const char* getTitle() { return "Game : Rain Of Chaos - Title"; }
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
		}
	}
}
