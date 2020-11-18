#pragma once

#include "step_pathfinder_game_TerrainData.h"

namespace step_pathfinder
{
	namespace tool
	{
		class TerrainData : public step_pathfinder::game::TerrainData
		{
		public:
			void Save( const char* file_name ) const;
		};
	}
}
