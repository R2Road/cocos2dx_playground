#pragma once

#include <list>

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

#include "cpg_Point.h"

NS_CC_BEGIN
	class Label;
NS_CC_END

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_loophero
{
	class TrimScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		TrimScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Trim"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void Build();
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cocos2d::Label* mSizeView;
		step_defender::game::TileMapNode* mTileMapNode;
		std::list<cpg::Point> mRoad;
	};
}
