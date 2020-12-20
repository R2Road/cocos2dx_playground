#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_flipflip
{
	namespace game
	{
		class TitleScene : public cocos2d::Scene
		{
		private:
			TitleScene();

		public:
			static const char* getTitle() { return "Game : Flip Flip"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

			void update4InputIndicator( float dt );

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Label* mRequestInputLabel;
			float mElapsedTime;
		};
	}
}
