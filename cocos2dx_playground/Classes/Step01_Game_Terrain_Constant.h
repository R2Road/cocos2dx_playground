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
				gate_entrance,
				gate_exit,
				gate_switch_on,
				gate_switch_off,

				SIZE,
				FIRST = damage,
			};
			 
			const char* TileType2TilePath( const eTileType tile_type );
		} // namespace pathfinder
	}
}
