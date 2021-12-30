#pragma once

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

#include "cpg_Position2GridIndexConverter.h"
#include "cpg_TileSheetConfiguration.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_astar
{
	class Grid4TileMap;

	class EditorNode : public cocos2d::Node
	{
	public:
		struct Config
		{
			int MapWidth = 10;
			int MapHeight = 10;
		};

	private:
		enum eToolIndex
		{
			Wall,
			Road,
			Entry,
			Exit,
		};

		EditorNode(
			const Config config
			, Grid4TileMap* const grid_4_tile_map
			, step_defender::game::TileMapNode* const tile_map_node
			, cocos2d::Node* const entry_point_indocator_node
			, cocos2d::Node* const exit_point_indocator_node
			, const cpg::TileSheetConfiguration& tile_sheet_configuration
		);

	public:
		static EditorNode* create(
			const Config config
			, Grid4TileMap* const grid_4_tile_map
			, step_defender::game::TileMapNode* const tile_map_node
			, cocos2d::Node* const entry_point_indocator_node
			, cocos2d::Node* const exit_point_indocator_node
			, const cpg::TileSheetConfiguration& tile_sheet_configuration
		);

	private:
		bool init() override;

		void onToolSelect( const int tool_index );
		void onGridClear( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

	private:
		void resetView();
		void updateDebugView();
		void updateEntryPointView();
		void updateExitPointView();
		void onUpdateTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

	private:
		const Config mConfig;

		Grid4TileMap* const mGrid4TileMap;
		step_defender::game::TileMapNode* const mTileMapNode;
		cocos2d::Node* const mEntryPointIndicatorNode;
		cocos2d::Node* const mExitPointIndicatorNode;
		cpg::Position2GridIndexConverter mPosition2GridIndexConverter;
		const cpg::TileSheetConfiguration mTileSheetConfiguration;

		int mToolIndex;
		step_defender::game::TileMapNode* mGridDebugViewNode;
	};
}
