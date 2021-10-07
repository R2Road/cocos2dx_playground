#pragma once

#include <functional>
#include <utility> // pair
#include <vector>

#include "cocos/2d/CCComponent.h"

#include "cpg_animation_InfoContainer.h"

NS_CC_BEGIN
	class Animate;
NS_CC_END

namespace step_mole
{
	class AnimationComponent : public cocos2d::Component
	{
	public:
		using AnimationCallback = std::function<void()>;

	private:
		using ParentT = cocos2d::Component;

		AnimationComponent();

	public:
		~AnimationComponent();

		static const char* GetStaticName() { static const char* COMPONENT_NAME = "CPG_ANIMATION"; return COMPONENT_NAME; }
		static AnimationComponent* create( const cpg_animation::InfoContainerT& animation_info_container );

		void PlayAnimation( const cpg_animation::eIndex animation_index );
		void PlayAnimationWithCallback( const cpg_animation::eIndex animation_index, const AnimationCallback animation_callback );
		void StopAnimation();

	private:
		bool init( const cpg_animation::InfoContainerT& animation_info_container );
		cocos2d::Animate* getAnimationAction( const cpg_animation::eIndex animation_index );

	private:
		std::vector<std::pair<cpg_animation::eIndex, cocos2d::Animate*>> mAnimationActions;
	};
}
