#pragma once

#include <vector>

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			enum class eTileType
			{
				damage,
				road,

				SIZE,
				FIRST = damage,
			};
			 
			const char* TileType2TilePath( const eTileType tile_type );
		} // namespace pathfinder
	}
}
