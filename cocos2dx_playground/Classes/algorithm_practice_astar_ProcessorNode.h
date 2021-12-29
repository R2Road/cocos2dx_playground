#pragma once

#include <list>

#include "2d/CCNode.h"

#include "algorithm_practice_astar_NodeAStar.h"
#include "cpg_Grid.h"
#include "cpg_TileSheetConfiguration.h"

namespace cpg_ui
{
	class ToolBarNode;
}

namespace algorithm_practice
{
	class Grid4TileMap;
}

namespace algorithm_practice_astar
{
	class CostMapNode;

	class ProcessorNode : public cocos2d::Node
	{
	public:
		struct Config
		{
			int MapWidth = 10;
			int MapHeight = 10;
		};

	private:
		enum class eMode
		{
			Step,
			Loop,
		};

		enum eStep
		{
			Entry,
			Loop,
			End,
		};

		ProcessorNode( const Config config, const cpg::TileSheetConfiguration& tile_sheet_configuration, const algorithm_practice::Grid4TileMap* const grid_4_tile_map );

	public:
		static ProcessorNode* create( const Config config, const cpg::TileSheetConfiguration& tile_sheet_configuration, const algorithm_practice::Grid4TileMap* const grid_4_tile_map );

	private:
		bool init() override;

		void onEnter() override;
		void onExit() override;

	public:
		void setVisible( bool visible ) override;

	private:
		void onModeSelect( const int mode_index );
		void updateCurrentPointView();

		void algorithmStep();
		void algorithmLoop( float dt );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		const Config mConfig;
		const cpg::TileSheetConfiguration mTileSheetConfiguration;
		const algorithm_practice::Grid4TileMap* const mGrid4TileMap;

		eMode mMode;
		float mElapsedTime4Loop;
		eStep mStep;

		std::list<Node4AStar> mOpenList;
		std::list<Node4AStar> mCloseList;
		cpg::Point mCurrentPoint;

		cpg_ui::ToolBarNode* mToolBarNode;
		cocos2d::Node* mCurrentPointIndicatorNode;
		CostMapNode* mCostMapNode;
	};
}
