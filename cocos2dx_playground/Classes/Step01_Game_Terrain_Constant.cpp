#include "Step01_Game_Terrain_Constant.h"

#include <array>

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			struct TileData
			{
				eTileType TileType = eTileType::damage;
				char* ResourcePath = "";
			};
			const std::array<TileData, static_cast<std::size_t>( eTileType::SIZE )> TileTable = { {
				{ eTileType::damage, "step01_game_tile_damage.png" }
				,{ eTileType::road, "step01_game_tile_road.png" }
				,{ eTileType::entrance, "step01_game_tile_entrance.png" }
				,{ eTileType::exit, "step01_game_tile_exit.png" }
				,{ eTileType::magic_circle_on, "step01_game_tile_magic_circle_01.png" }
				,{ eTileType::magic_circle_off, "step01_game_tile_magic_circle_02.png" }
			} };

			const char* TileType2TilePath( const eTileType tile_type )
			{
				if( eTileType::FIRST > tile_type || eTileType::SIZE <= tile_type )
				{
					return TileTable[0u].ResourcePath;
				}

				return TileTable[static_cast<std::size_t>( tile_type )].ResourcePath;
			}
		}
	}
}
