#include "Step01_Game_PathFinder_TerrainData.h"

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			TerrainData::TerrainData( const int width, const int height ) : container( width * height, false ) {}
		} // namespace pathfinder
	}
}
