#pragma once

namespace step_pathfinder
{
	namespace game
	{
		class TerrainData;
	}

	namespace tool
	{
		class TerrainSaveHelper
		{
		public:
			static void Save( const game::TerrainData& terrain_data, const char* file_path );
		};
	}
}
