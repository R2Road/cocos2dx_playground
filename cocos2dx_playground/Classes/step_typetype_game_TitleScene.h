#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_typetype
{
	namespace game
	{
		class TitleScene : public cocos2d::Scene
		{
		private:
			TitleScene();

		public:
			static const char* getTitle() { return "Game : Type Type"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

			void update4Letters( float dt );
			void update4InputIndicator( float dt );

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			int mStep4Letters;
			float mElapsedTime4Letters;
			int mCurrentLetterTag;

			cocos2d::Label* mInputIndicatorLabel;
			float mElapsedTime4InputIndicator;
		};
	}
}
