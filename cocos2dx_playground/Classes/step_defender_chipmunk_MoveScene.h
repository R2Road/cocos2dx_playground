#pragma once

//
// Documentation
// - https://chipmunk-physics.net/release/ChipmunkLatest-Docs/
//

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollector.h"

namespace step_defender
{
	namespace chipmunk
	{
		class MoveScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			MoveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Chipmunk : Move"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void update4Input( float delta_time );

			void addStaticPhysicsBody( const cocos2d::Vec2 sprite_position );
			void updateMoveSpeedView();
			void updatePhysicsRotationFlagView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;

			int mMoveSpeed;
			cocos2d::Node* mMoveNode;
		};
	}
}
