#pragma once

#include <functional>

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
		using ProcessExitCallback = std::function<void( int )>;

		enum class eState
		{
			Wait,

			Wakeup,
			Action,
			Sleep,
			
			Damaged_1,
			Damaged_2,

			Exit,
		};

		ObjectComponent(
			AnimationComponent* const animation_component
			, cocos2d::Component* const circle_collision_component
			, const ProcessExitCallback& process_end_callback
		);

	public:
		static const char* GetStaticName() { return "MOLE_OBJECT"; }
		static ObjectComponent* create(
			AnimationComponent* const animation_component
			, cocos2d::Component* const circle_collision_component
			, const ProcessExitCallback& process_end_callback
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
		const ProcessExitCallback mProcessEndCallback;

		float mActionTime;
	};
}
