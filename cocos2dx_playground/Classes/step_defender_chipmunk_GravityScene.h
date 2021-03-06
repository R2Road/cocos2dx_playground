#pragma once

//
// Documentation
// - https://chipmunk-physics.net/release/ChipmunkLatest-Docs/
//

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

namespace step_defender
{
	namespace chipmunk
	{
		class GravityScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			GravityScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Chipmunk : Gravity"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void addPhysicsBody( const cocos2d::Vec2 sprite_position, const bool is_dynamic );

			void onGravityHelperButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void updateGravityView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::Vec2 mGravityHelperButton_MoveOffset;
		};
	}
}
