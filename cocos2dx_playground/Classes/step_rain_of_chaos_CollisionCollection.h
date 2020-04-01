#pragma once

#include <list>

namespace step_mole
{
	class CircleCollisionComponent;
}

namespace step_rain_of_chaos
{
	class CollisionCollection
	{
	public:
		void Add( step_mole::CircleCollisionComponent* component );
		void Remove( step_mole::CircleCollisionComponent* component );
		void Clear() { mCollisionList.clear(); }

	private:
		std::list<step_mole::CircleCollisionComponent*> mCollisionList;
	};
}
