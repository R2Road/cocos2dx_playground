#pragma once

#include <functional>

#include "2d/CCNode.h"
#include "2d/CCComponent.h"
#include "base/CCRefPtr.h"
#include "math/CCGeometry.h"

namespace step_mole
{
	class AnimationComponent;
}

namespace step_rain_of_chaos
{
	namespace game
	{
		class BulletLifeComponent : public cocos2d::Component
		{
		private:
			using ParentT = cocos2d::Component;
			using ProcessExitCallback = std::function<void( int )>;

			enum class eState
			{
				Sleep,

				Wakeup,
				Move,
				Disappear,

				Boom_1,
				Boom_2,

				Exit,
			};

			BulletLifeComponent(
				const cocos2d::Rect allowed_area
				, step_mole::AnimationComponent* const animation_component
				, cocos2d::Component* const circle_collision_component
				, const ProcessExitCallback& process_end_callback
			);

		public:
			static const char* GetStaticName() { return "ROC_BULLETLIFE"; }
			static BulletLifeComponent* create(
				const cocos2d::Rect allowed_area
				, step_mole::AnimationComponent* const animation_component
				, cocos2d::Component* const circle_collision_component
				, const ProcessExitCallback& process_end_callback
			);

			bool init() override;

			void ProcessStart( const cocos2d::Vec2 new_position, const cocos2d::Vec2 move_direction );
			void ProcessBoom();

		private:
			void ChangeState( const eState next_state );

		private:
			const cocos2d::Rect mAllowedArea;

			eState mLastState;
			cocos2d::Vec2 mMoveDirection;

			step_mole::AnimationComponent* const mAnimationComponent;
			cocos2d::Component* const mCircleCollisionComponent;
			const ProcessExitCallback mProcessEndCallback;
		};
	}
}
