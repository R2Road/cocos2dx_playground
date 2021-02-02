#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "algorithm_practice_floodfill_Constant.h"
#include "algorithm_practice_floodfill_Grid4TileMap.h"
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
	class EditorNode;

	class TestScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		enum eMode
		{
			Edit,
			Process,
		};

		enum eStep
		{
			Entry,
			Loop,
			End,
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
		void onModeSelect( const int mode_index );	

		void updateCurrentPointView();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		eMode mMode;

		cpg::TileSheetConfiguration mTileSheetConfiguration;
		Grid4TileMap mGrid4TileMap;

		step_defender::game::TileMapNode* mTileMapNode;
		DirectionMapNode* mDirectionMapNode;
		EditorNode* mEditorNode;

		eStep mStep;
		cpg::Grid<Cell4FloodFill> mGrid4FloodFill;
		cpg::Point mCurrentPoint;
		cocos2d::Node* mCurrentPointIndicatorNode;
	};
}
