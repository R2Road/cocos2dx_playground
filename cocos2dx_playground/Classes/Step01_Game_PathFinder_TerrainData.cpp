#include "Step01_Game_PathFinder_TerrainData.h"

#include <random>

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			TerrainData::TerrainData( const int width, const int height ) : container( width * height, false ) {}

			void TerrainData::load()
			{
				//
				// generate dummy data
				//
				std::random_device rd;
				std::mt19937 randomEngine( rd() );
				std::uniform_int_distribution<> dist( 0, 1 );

				for( auto& d : container )
				{
					d = dist( randomEngine );
				}
			}
		} // namespace pathfinder
	}
}
