#include "Step01_Game_Terrain_Constant.h"

#include <array>

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			const std::array<TileData, static_cast<std::size_t>( eTileType::SIZE )> TileTable = { {
				{ eTileType::damage, "step01_game_tile_damage.png", true, "Damage Tile" }
				,{ eTileType::road, "step01_game_tile_road.png", true, "Road Tile" }
				,{ eTileType::entrance, "step01_game_tile_entrance.png", true, "Entrance" }
				,{ eTileType::exit, "step01_game_tile_exit.png", true, "Exit" }
				,{ eTileType::magic_circle_on, "step01_game_tile_magic_circle_01.png", true, "Switch" }
				,{ eTileType::magic_circle_off, "step01_game_tile_magic_circle_02.png", false, "" }
			} };

			const TileData& TileType2TileData( const eTileType tile_type )
			{
				if( eTileType::FIRST > tile_type || eTileType::SIZE <= tile_type )
				{
					static const TileData dummy{ eTileType::damage, "step01_game_tile_damage.png", false, "W T F" };
					return dummy;
				}

				return TileTable[static_cast<std::size_t>( tile_type )];;
			}
		}
	}
}
