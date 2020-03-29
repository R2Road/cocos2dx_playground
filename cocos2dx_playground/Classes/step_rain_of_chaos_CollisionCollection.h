#pragma once

#include <list>

namespace step_mole
{
	class CollisionComponent;
}

namespace step_rain_of_chaos
{
	class CollisionCollection
	{
	public:
		void Add( step_mole::CollisionComponent* component );
		void Remove( step_mole::CollisionComponent* component );
		void Clear() { mCollisionList.clear(); }

	private:
		std::list<step_mole::CollisionComponent*> mCollisionList;
	};
}
