#pragma once

#include "cocos/2d/CCComponent.h"

namespace cpg
{
	class CollisionComponent : public cocos2d::Component
	{
	private:
		CollisionComponent( const float radius );

	public:
		static CollisionComponent* create( const float radius );

		float GetRadius() const { return mRadius; }

	private:
		const float mRadius;
	};
}
