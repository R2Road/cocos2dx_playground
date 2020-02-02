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
				static const char* road_tile_gate_entrance = "step01_game_obj_gate_entrance.png";
				static const char* road_tile_gate_exit = "step01_game_obj_gate_exit.png";
				static const char* road_tile_gate_switch_on = "step01_game_obj_gate_switch_01.png";
				static const char* road_tile_gate_switch_off = "step01_game_obj_gate_switch_02.png";

				if( eTileType::damage == tile_type )
				{
					return damaged_tile_path;
				}
				else if( eTileType::road == tile_type )
				{
					return road_tile_path;
				}
				else if( eTileType::gate_entrance == tile_type )
				{
					return road_tile_gate_entrance;
				}
				else if( eTileType::gate_exit == tile_type )
				{
					return road_tile_gate_exit;
				}
				else if( eTileType::gate_switch_on == tile_type )
				{
					return road_tile_gate_switch_on;
				}
				else if( eTileType::gate_switch_off == tile_type )
				{
					return road_tile_gate_switch_off;
				}

				return damaged_tile_path;
			}
		}
	}
}
