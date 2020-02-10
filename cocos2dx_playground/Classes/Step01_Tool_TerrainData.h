#pragma once

#include "Step01_Game_TerrainData.h"

namespace step01
{
	namespace tool
	{
		class TerrainData : public step01::game::TerrainData
		{
		public:
			void save( const char* file_name );
		};
	}
}
