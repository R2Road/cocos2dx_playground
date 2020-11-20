#include "step_pathfinder_tool_TerrainSaveHelper.h"

#include <fstream>

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace step_pathfinder
{
	namespace tool
	{
		void TerrainSaveHelper::Save( const game::TerrainData& terrain_data, const char* file_name )
		{
			rapidjson::Document document;
			document.SetArray();
			for( const auto& row : terrain_data )
			{
				for( const auto& tile_type : row )
				{
					document.PushBack( static_cast<int>( tile_type ), document.GetAllocator() );
				}
			}

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
			document.Accept( writer );

			std::ofstream fs( file_name, std::ios::out );
			if( fs.fail() )
			{
				CCLOG( "Failed : Terrain Data Save" );
				return;
			}

			fs << buffer.GetString() << std::endl;
			fs.close();
		}
	}
}
