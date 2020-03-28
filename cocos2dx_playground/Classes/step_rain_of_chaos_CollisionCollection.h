#pragma once

#include <list>

namespace step_rain_of_chaos
{
	class CollisionComponent;

	class CollisionCollection
	{
	public:
		void Add( CollisionComponent* component );
		void Remove( CollisionComponent* component );
		void Clear() { mCollisionList.clear(); }

	private:
		std::list<CollisionComponent*> mCollisionList;
	};
}
