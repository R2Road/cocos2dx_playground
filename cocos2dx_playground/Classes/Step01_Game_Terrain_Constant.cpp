#include "Step01_Game_Terrain_Constant.h"

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			const char* TileType2TilePath( const eTileType tile_type )
			{
				static const char* damage_tile_path = "step01_game_tile_damage.png";
				static const char* road_tile_path = "step01_game_tile_road.png";
				static const char* entrance_tile_path = "step01_game_tile_entrance.png";
				static const char* exit_tile_path = "step01_game_tile_exit.png";
				static const char* magic_ciecle_on_tile_path = "step01_game_tile_magic_circle_01.png";
				static const char* magic_ciecle_off_tile_path = "step01_game_tile_magic_circle_02.png";

				if( eTileType::damage == tile_type )
				{
					return damage_tile_path;
				}
				else if( eTileType::road == tile_type )
				{
					return road_tile_path;
				}
				else if( eTileType::entrance == tile_type )
				{
					return entrance_tile_path;
				}
				else if( eTileType::exit == tile_type )
				{
					return exit_tile_path;
				}
				else if( eTileType::magic_circle_on == tile_type )
				{
					return magic_ciecle_on_tile_path;
				}
				else if( eTileType::magic_circle_off == tile_type )
				{
					return magic_ciecle_off_tile_path;
				}

				return damage_tile_path;
			}
		}
	}
}
