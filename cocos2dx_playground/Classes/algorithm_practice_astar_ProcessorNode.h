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

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_astar
{
	class CostMapNode;
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
		enum class eMode
		{
			Step,
			Loop,
		};

		enum eStep
		{
			Entry,
			Loop,
			Result,
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
		void onModeSelect( const int mode_index );

		void algorithmStep();
		void algorithmLoop( float dt );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		const Config mConfig;
		const cpg::TileSheetConfiguration mTileSheetConfiguration;
		const Grid4TileMap* const mGrid4TileMap;

		eMode mMode;
		float mElapsedTime4Loop;
		eStep mStep;

		using Node4AStarContainerT = std::list<Node4AStar>;
		Node4AStarContainerT mOpenList;
		Node4AStarContainerT mCloseList;
		Node4AStarContainerT mUpdateList;
		cpg::Point mCurrentPoint;

		cpg_ui::ToolBarNode* mToolBarNode;
		CostMapNode* mCostMapNode;
		step_defender::game::TileMapNode* mPathNode;
	};
}
