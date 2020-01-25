#pragma once

#include <vector>

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			class TerrainData
			{
			public:
				TerrainData();

				void load( const int width, const int height );

				const std::vector<bool> get() const { return container; }

			private:
				std::vector<bool> container;
			};
		} // namespace pathfinder
	}
}
