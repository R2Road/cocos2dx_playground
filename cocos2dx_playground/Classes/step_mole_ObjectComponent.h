#pragma once

#include "cocos/2d/CCNode.h"
#include "cocos/2d/CCComponent.h"
#include "cocos/base/CCRefPtr.h"

namespace cpg
{
	class AnimationComponent;
}

namespace step_mole
{
	class ObjectComponent : public cocos2d::Component
	{
	private:
		using ParentT = cocos2d::Component;

		enum class eState
		{
			Wakeup,
			Action,
			Sleep,
			
			Damaged,

			Hide,
		};

		ObjectComponent( cpg::AnimationComponent* const animation_component );

	public:
		static const char* GetStaticName() { return "MOLE_OBJECT"; }
		static ObjectComponent* create( cpg::AnimationComponent* const animation_component );

		bool init() override;
		void setEnabled( bool enabled ) override;

	private:
		void ChangeState( const eState next_state );

	private:
		eState mLastState;
		cpg::AnimationComponent* const mAnimationComponent;
	};
}
