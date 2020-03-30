#include "step_rain_of_chaos_CollisionCollection.h"

namespace step_rain_of_chaos
{
	void CollisionCollection::Add( step_mole::CircleCollisionComponent* component )
	{
		mCollisionList.push_back( component );
	}
	void CollisionCollection::Remove( step_mole::CircleCollisionComponent* component )
	{
		mCollisionList.remove( component );
	}
}
