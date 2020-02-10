#pragma once

#include "Step01_Game_TerrainData.h"

namespace step01
{
	namespace tool
	{
		class TerrainData : public step01::game::TerrainData
		{
		public:
			void fill( const Row::value_type tile_type );

			void save( const char* file_name );
			bool load( const char* file_name );
		};
	}
}
