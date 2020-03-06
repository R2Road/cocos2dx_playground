#pragma once

#include "cocos/2d/CCComponent.h"

namespace cpg
{
	class AnimationComponent : public cocos2d::Component
	{
	private:
		using ParentT = cocos2d::Component;

		AnimationComponent();

	public:
		static const char* GetStaticName() { static const char* COMPONENT_NAME = "CPG_ANIMATION"; return COMPONENT_NAME; }
		static AnimationComponent* create();
	};
}
