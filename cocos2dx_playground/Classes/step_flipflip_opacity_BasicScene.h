#pragma once

#include "2d/CCScene.h"

NS_CC_BEGIN;
	class Label;
NS_CC_END;

namespace step_flipflip
{
	namespace opacity
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Opacity : Basic"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

			void update4Opacity( float dt );

		private:
			void updateOpacityView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Node* mTestNode;
			cocos2d::Label* mOpacityView;

			enum eOpacityFlag
			{
				Up,
				Down,
			};
			int mOpacityFlags;
			float mCurrentOpacity;
		};
	}
}
