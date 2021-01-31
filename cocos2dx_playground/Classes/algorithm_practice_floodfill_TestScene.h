#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "algorithm_practice_floodfill_Constant.h"
#include "algorithm_practice_floodfill_Grid4TileMap.h"
#include "cpg_Position2GridIndexConverter.h"
#include "cpg_TileSheetConfiguration.h"
#include "helper_BackToThePreviousScene.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_floodfill
{
	class DirectionMapNode;

	class TestScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		enum eStep
		{
			Entry,
			Loop,
		};

		TestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Test"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onGridClear( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void ResetView();

		void onToolSelect( const int tool_index );
		void onUpdateTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void updateDebugView();
		void updateEntryPointView();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cpg::TileSheetConfiguration mTileSheetConfiguration;
		cpg::Position2GridIndexConverter mPosition2GridIndexConverter;
		Grid4TileMap mGrid4TileMap;
		step_defender::game::TileMapNode* mTileMapNode;
		int mToolIndex;

		cocos2d::Node* mEntryPointIndicatorNode;
		step_defender::game::TileMapNode* mGridDebugViewNode;

		DirectionMapNode* mDirectionMapNode;

		eStep mStep;
		cpg::Grid<Cell4FloodFill> mGrid4Direction;
		cpg::Point mCurrentPoint;
	};
}
