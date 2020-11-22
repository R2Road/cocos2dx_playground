#pragma once

#include "math/CCGeometry.h"

namespace step_pathfinder
{
	namespace game
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
		cocos2d::Size GetMaxMenuSize();

		float CalculateTileScale( const float tile_height );


		struct TerrainPoint
		{
			int x = 0;
			int y = 0;
		};
	}
}
