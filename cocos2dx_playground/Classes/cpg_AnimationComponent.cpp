#include "cpg_AnimationComponent.h"

#include <new>

USING_NS_CC;

namespace cpg
{
	AnimationComponent::AnimationComponent()
	{
		setName( GetStaticName() );
	}

	AnimationComponent* AnimationComponent::create()
	{
		auto ret = new ( std::nothrow ) AnimationComponent();
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}
}
