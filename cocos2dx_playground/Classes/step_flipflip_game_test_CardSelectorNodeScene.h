#pragma once

#include "2d/CCScene.h"

namespace step_flipflip
{
	namespace game
	{
		class CardSelectorNode;
	}

	namespace game_test
	{
		class CardSelectorNodeScene : public cocos2d::Scene
		{
		private:
			CardSelectorNodeScene();

		public:
			static const char* getTitle() { return "Game Test : Card Selector Node"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateSelectedIndicatorPositionView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::CardSelectorNode* mCardSelectorNode;
		};
	}
}
