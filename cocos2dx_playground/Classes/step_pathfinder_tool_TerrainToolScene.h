#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

#include "step_pathfinder_game_TerrainData.h"

namespace step_pathfinder
{
	namespace game
	{
		class TerrainViewer;
	}

	namespace tool
	{
		class TerrainEditHelper;

		class TerrainToolScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			TerrainToolScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Tool : Terrain"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			cocos2d::Node* makeMenuButton( const cocos2d::Size menu_size, const step_pathfinder::game::eTileType tile_type, const char* button_text, const cocos2d::ui::Widget::ccWidgetTouchCallback& callback );
			void onTileSelect( const step_pathfinder::game::eTileType new_tile_type );
			void onGrid( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onSave( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void onLoad( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			game::TerrainData mTerrainData;
			step_pathfinder::game::eTileType mCurrentTileType;

			game::TerrainViewer* mTerrainViewer;
		};
	}
}
