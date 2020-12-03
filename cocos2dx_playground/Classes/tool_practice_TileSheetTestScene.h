#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"
#include "helper_BackToThePreviousScene.h"

#include "cpg_GridIndexConverter.h"

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
		TileSheetTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Tool : Parallax Layer Editor"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onToolSelect( const int tool_index );
		void onTileSelect( const int x, const int y );

		void onUpdateTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void onAddTile( const int grid_point_x, const int grid_point_y );
		void onEraseTile( const int grid_point_x, const int grid_point_y );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		cpg::GridIndexConverter mGridIndexConverter;

		step_defender::game::TileMapNode* mTileMapNode;

		struct
		{
			int x;
			int y;
		} mCurrentTilePoint;
		int mToolIndex;
	};
}
