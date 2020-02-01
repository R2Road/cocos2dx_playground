#include "Step01_Game_Terrain_Constant.h"

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			const char* TileType2TilePath( const eTileType tile_type )
			{
				static const char* damaged_tile_path = "step01_game_tile_00.png";
				static const char* road_tile_path = "step01_game_tile_01.png";

				if( eTileType::damage == tile_type )
				{
					return damaged_tile_path;
				}
				else if( eTileType::road == tile_type )
				{
					return road_tile_path;
				}

				return damaged_tile_path;
			}
		}
	}
}
