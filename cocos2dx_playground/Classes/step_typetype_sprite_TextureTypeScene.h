#pragma once

#include "cocos2d.h"

namespace step_typetype
{
	namespace sprite
	{
		class TextureTypeScene : public cocos2d::Scene
		{
		public:
			TextureTypeScene();

			static const char* getTitle() { return "Sprite : Texture Type"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			int mPressedKeyCount;
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
