#pragma once

#include <vector>

#include "cocos/2d/CCComponent.h"

#include "cpg_Animation_Info.h"

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
		static AnimationComponent* create( const std::vector<cpg::animation::Info>& animation_infos );

		void PlayAnimation( const cpg::animation::eIndex animation_index );
		void StopAnimation();

	private:
		cocos2d::Action* getAnimationAction( const cpg::animation::eIndex animation_index );

	private:
		std::vector<cocos2d::Action*> mAnimationActions;
	};
}
