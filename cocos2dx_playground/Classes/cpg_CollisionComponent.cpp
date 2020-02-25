#include "cpg_CollisionComponent.h"

#include <new>

USING_NS_CC;

namespace cpg
{
	CollisionComponent::CollisionComponent( const float radius ) : mRadius( radius )
	{
		setName( "CPG_COLLISION" );
	}

	CollisionComponent* CollisionComponent::create( const float radius )
	{
		auto ret = new ( std::nothrow ) CollisionComponent( radius );
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
