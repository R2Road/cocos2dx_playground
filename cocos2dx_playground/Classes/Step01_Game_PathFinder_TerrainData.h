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
				TerrainData( const int width, const int height );

			private:
				std::vector<bool> container;
			};
		} // namespace pathfinder
	}
}
