#pragma once

#include <vector>

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace graph_practice
{
	class GraphAndNameNode;

	class Collection03Scene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		Collection03Scene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Collection 03"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void update4Graph( float dt );
		void updateTimeScaleView();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		float mTimeScale;
		float mElapsedTime;
		bool mbPause;

		std::vector<GraphAndNameNode*> mGraphViewNodeContainer;
	};
}
