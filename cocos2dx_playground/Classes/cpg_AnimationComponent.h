#pragma once

#include <functional>
#include <utility> // pair
#include <vector>

#include "cocos/2d/CCComponent.h"

#include "cpg_animation_InfoContainer.h"

NS_CC_BEGIN
	class Animate;
NS_CC_END

namespace cpg
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
		static AnimationComponent* create( const cpg::animation::InfoContainer& animation_info_container );

		void PlayAnimation( const cpg::animation::eIndex animation_index );
		void PlayAnimationWithCallback( const cpg::animation::eIndex animation_index, const AnimationCallback animation_callback );
		void StopAnimation();

	private:
		bool init( const cpg::animation::InfoContainer& animation_info_container );
		cocos2d::Animate* getAnimationAction( const cpg::animation::eIndex animation_index );

	private:
		std::vector<std::pair<cpg::animation::eIndex, cocos2d::Animate*>> mAnimationActions;
	};
}
