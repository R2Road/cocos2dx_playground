#pragma once

#include "cocos/2d/CCNode.h"
#include "cocos/2d/CCComponent.h"
#include "cocos/base/CCRefPtr.h"

namespace step_mole
{
	class AnimationComponent;

	class ObjectComponent : public cocos2d::Component
	{
	private:
		using ParentT = cocos2d::Component;

		enum class eState
		{
			Wait,

			Wakeup,
			Action,
			Sleep,
			
			Damaged_1,
			Damaged_2,
		};

		ObjectComponent(
			AnimationComponent* const animation_component
			, cocos2d::Component* const circle_collision_component
		);

	public:
		static const char* GetStaticName() { return "MOLE_OBJECT"; }
		static ObjectComponent* create(
			AnimationComponent* const animation_component
			, cocos2d::Component* const circle_collision_component
		);

		bool init() override;
		void onAdd() override;

		void ProcessStart( const float action_time );
		void ProcessDamage();

	private:
		void ChangeState( const eState next_state );

	private:
		eState mLastState;
		AnimationComponent* const mAnimationComponent;
		cocos2d::Component* const mCircleCollisionComponent;

		float mActionTime;
	};
}
