#pragma once

#include "2d/CCNode.h"

#include "algorithm_practice_floodfill_Constant.h"
#include "cpg_Grid.h"
#include "cpg_TileSheetConfiguration.h"

namespace algorithm_practice_floodfill
{
	class DirectionMapNode;
	class Grid4TileMap;

	class ProcessorNode : public cocos2d::Node
	{
	public:
		struct Config
		{
			int MapWidth = 10;
			int MapHeight = 10;
		};

	private:
		enum eMode
		{
			Step,
		};

		enum eStep
		{
			Entry,
			Loop,
			End,
		};

		ProcessorNode( const Config config, const cpg::TileSheetConfiguration& tile_sheet_configuration, const Grid4TileMap* const grid_4_tile_map );

	public:
		static ProcessorNode* create( const Config config, const cpg::TileSheetConfiguration& tile_sheet_configuration, const Grid4TileMap* const grid_4_tile_map );

	private:
		bool init() override;

		void onEnter() override;
		void onExit() override;

	public:
		void setVisible( bool visible ) override;

	private:
		void updateCurrentPointView();

		void algorithmStep();

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		const Config mConfig;
		const cpg::TileSheetConfiguration mTileSheetConfiguration;
		const Grid4TileMap* const mGrid4TileMap;

		eMode mMode;
		eStep mStep;
		cpg::Grid<Cell4FloodFill> mGrid4FloodFill;
		cpg::Point mCurrentPoint;

		cocos2d::Node* mCurrentPointIndicatorNode;
		DirectionMapNode* mDirectionMapNode;
	};
}
