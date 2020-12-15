#pragma once

#include <vector>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"
#include "helper_BackToThePreviousScene.h"

#include "cpg_Position2GridIndexConverter.h"

#include "tool_practice_TileSheetTestConfiguration.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace tool_practice
{
	class TileSheetTestScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		enum class eToolIndex
		{
			Pick,
			Erase,
		};

		TileSheetTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Tile Sheet Test"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void reloadTexture();
		void onLayerSelect( const int layer_index );
		void onToolSelect( const int tool_index );
		void onTileSelect( const int x, const int y );

		void onUpdateTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void onAddTile( const int grid_point_x, const int grid_point_y );
		void onEraseTile( const int grid_point_x, const int grid_point_y );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		TileSheetTestConfiguration mConfiguration;

		cpg::Position2GridIndexConverter mPosition2GridIndexConverter;

		std::vector<step_defender::game::TileMapNode*> mTileMapNodeContainer;
		step_defender::game::TileMapNode* mTileMapNode;

		struct
		{
			int x;
			int y;
		} mCurrentTilePoint;
		eToolIndex mToolIndex;
	};
}
