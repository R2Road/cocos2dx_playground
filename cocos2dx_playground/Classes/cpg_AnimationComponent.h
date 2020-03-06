#pragma once

#include <vector>

#include "cocos/2d/CCComponent.h"

#include "cpg_animation_InfoContainer.h"

namespace cpg
{
	class AnimationComponent : public cocos2d::Component
	{
	private:
		using ParentT = cocos2d::Component;

		AnimationComponent();
		~AnimationComponent();

	public:
		static const char* GetStaticName() { static const char* COMPONENT_NAME = "CPG_ANIMATION"; return COMPONENT_NAME; }
		static AnimationComponent* create( const cpg::animation::InfoContainer& animation_info_container );

		void PlayAnimation( const cpg::animation::eIndex animation_index );
		void StopAnimation();

	private:
		bool init( const cpg::animation::InfoContainer& animation_info_container );
		cocos2d::Action* getAnimationAction( const cpg::animation::eIndex animation_index );

	private:
		std::vector<cocos2d::Action*> mAnimationActions;
	};
}
