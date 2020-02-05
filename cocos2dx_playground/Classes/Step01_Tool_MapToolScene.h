#pragma once

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "Step01_Tool_TerrainData.h"

namespace step01
{
	namespace tool
	{
		class MapToolScene : public cocos2d::Scene
		{
		private:
			MapToolScene();

		public:
			static const char* getTitle() { return "Tool : Map Tool"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			Node* makeMenuButton( const step01::game::terrain::eTileType tile_type, const char* button_text, const cocos2d::ui::Widget::ccWidgetTouchCallback& callback );
			void onTileSelect( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void onGrid( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onSave( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			TerrainData mTerrainData;
			step01::game::terrain::eTileType mCurrentTileType;
			cocos2d::Node* mButtonRootNode;
			cocos2d::Node* terrain_layer;
		};
	}
}
