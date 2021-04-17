#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_loophero
{
	class RootScene : public cocos2d::Scene
	{
	private:
		RootScene();

	public:
		static const char* getTitle() { return "Loop Hero( In Progress )"; }
		static cocos2d::Scene* create();

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
