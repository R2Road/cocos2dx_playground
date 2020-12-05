#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"
#include "helper_BackToThePreviousScene.h"

#include "cpg_Position2GridIndexConverter.h"
#include "step_rain_of_chaos_input_KeyCodeCollector.h"

NS_CC_BEGIN
class ParallaxNode;
NS_CC_END

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}

	namespace tool
	{
		class ParallaxLayerEditorScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			ParallaxLayerEditorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Tool : Parallax Layer Editor"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

			void update4Move( float delta_time );

		private:
			void onLayerSelect( const int layer_index );
			void onToolSelect( const int tool_index );
			void onTileSelect( const int x, const int y );

			void onUpdateTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void onAddTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void onEraseTile( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;
			cpg::Position2GridIndexConverter mPosition2GridIndexConverter;

			cocos2d::ParallaxNode* mRulerNode;
			cocos2d::ParallaxNode* mParallaxNode;
			cocos2d::Node* mTouchNode;
			game::TileMapNode* mCurrentBackgroundLayer;

			struct
			{
				int x;
				int y;
			} mCurrentTilePoint;
			int mToolIndex;
		};
	}
}
