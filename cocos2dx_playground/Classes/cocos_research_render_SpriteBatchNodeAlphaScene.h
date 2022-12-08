#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
	class Label;
	class SpriteBatchNode;
NS_CC_END

namespace cocos_research_render
{
	class SpriteBatchNodeAlphaScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		SpriteBatchNodeAlphaScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Sprite Batch Node"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cocos2d::SpriteBatchNode* mSpriteBatchNode;
		cocos2d::Label* mTileMapOpacityLabel;
	};
}
