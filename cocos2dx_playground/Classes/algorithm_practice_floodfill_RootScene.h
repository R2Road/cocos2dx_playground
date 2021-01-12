#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "algorithm_practice_floodfill_Constant.h"
#include "cpg_Grid.h"
#include "cpg_Position2GridIndexConverter.h"
#include "helper_BackToThePreviousScene.h"
#include "tool_practice_TileSheetTestConfiguration.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_floodfill
{
	class RootScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		RootScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Flood Fill"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onToolSelect( const int tool_index );
		void onUpdateTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void onUpdateDebugView();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		tool_practice::TileSheetTestConfiguration mConfiguration;
		cpg::Position2GridIndexConverter mPosition2GridIndexConverter;
		cpg::Grid<GridValue> mGrid;
		step_defender::game::TileMapNode* mTileMapNode;
		int mToolIndex;
		cpg::Point mEntryPoint;

		cocos2d::Node* mEntryPointIndicatorNode;
		step_defender::game::TileMapNode* mGridDebugViewNode;
	};
}
