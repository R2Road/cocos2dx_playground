#pragma once

//
// Documentation
// - https://chipmunk-physics.net/release/ChipmunkLatest-Docs/
//

#include "2d/CCScene.h"
#include "helper_BackToThePreviousScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollector.h"

NS_CC_BEGIN
class EventListenerPhysicsContact;
class PhysicsContact;
class PhysicsContactPostSolve;
class PhysicsContactPreSolve;
NS_CC_END

namespace step_defender
{
	namespace chipmunk
	{
		class ContactScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			ContactScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Chipmunk : Contact"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void update4Input( float delta_time );

			bool onContactBegin( cocos2d::PhysicsContact& contact );
			bool onContactPreSolve( cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve );
			void onContactPostSolve( cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve );
			void onContactSeparate( cocos2d::PhysicsContact& contact );

			void addStaticPhysicsBody( const cocos2d::Vec2 sprite_position );
			void addStaticPhysicsSensor( const cocos2d::Vec2 sprite_position );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			step_rain_of_chaos::input::KeyCodeCollector mKeyCodeCollector;

			cocos2d::EventListenerPhysicsContact* mEventListenerPhysicsContact;

			const int mMoveSpeed;
			cocos2d::Node* mMoveNode;
		};
	}
}
