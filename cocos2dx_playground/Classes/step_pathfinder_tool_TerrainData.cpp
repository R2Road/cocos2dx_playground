#include "step_pathfinder_tool_TerrainData.h"

#include <fstream>

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace step_pathfinder
{
	namespace tool
	{
		void TerrainData::save( const char* file_name )
		{
			rapidjson::Document document;
			document.SetArray();
			for( auto& row : mContainer )
			{
				for( auto& t : row )
				{
					document.PushBack( static_cast<int>( t ), document.GetAllocator() );
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
