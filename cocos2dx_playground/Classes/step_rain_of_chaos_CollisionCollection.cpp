#include "step_rain_of_chaos_CollisionCollection.h"

namespace step_rain_of_chaos
{
	void CollisionCollection::Add( CollisionComponent* component )
	{
		mCollisionList.push_back( component );
	}
	void CollisionCollection::Remove( CollisionComponent* component )
	{
		mCollisionList.remove( component );
	}
}
