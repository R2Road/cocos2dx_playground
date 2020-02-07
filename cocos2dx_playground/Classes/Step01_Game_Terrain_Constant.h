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
				entrance,
				exit,
				magic_circle_on,
				magic_circle_off,

				SIZE,
				FIRST = damage,
			};

			struct TileData
			{
				eTileType TileType = eTileType::damage;
				bool bToolEnable = false;
				bool bUnique = false;
				char* ResourcePath = "";
				char* Name = "";
			};
			const TileData& TileType2TileData( const eTileType tile_type );
			bool TileType2UniqueFlag( const eTileType tile_type );
		} // namespace pathfinder
	}
}
