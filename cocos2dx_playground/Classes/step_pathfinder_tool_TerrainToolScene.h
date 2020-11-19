#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "step_pathfinder_tool_TerrainData.h"

namespace step_pathfinder
{
	namespace game
	{
		class TerrainViewer;
	}

	namespace tool
	{
		class TerrainEditHelper;

		class TerrainToolScene : public cocos2d::Scene
		{
		private:
			TerrainToolScene();

		public:
			static const char* getTitle() { return "Tool : Terrain"; }
			static cocos2d::Scene* create();

			bool init() override;
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
			TerrainData mTerrainData;
			step_pathfinder::game::eTileType mCurrentTileType;

			game::TerrainViewer* mTerrainViewer;
		};
	}
}
