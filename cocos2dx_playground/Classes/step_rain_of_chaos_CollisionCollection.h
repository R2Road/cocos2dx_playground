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
		using ContainerT = std::list<step_mole::CircleCollisionComponent*>;

		void Add( step_mole::CircleCollisionComponent* component );
		void Remove( step_mole::CircleCollisionComponent* component );
		void Clear() { mContainer.clear(); }

		const std::size_t Count() const { return mContainer.size(); }

	private:
		ContainerT mContainer;
	};
}
