#include "step_pathfinder_tool_TerrainSaveHelper.h"

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "step_pathfinder_game_TerrainData.h"

namespace step_pathfinder
{
	namespace tool
	{
		void TerrainSaveHelper::Save( const game::TerrainData& terrain_data, const char* file_path )
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

			cocos2d::FileUtils::getInstance()->writeStringToFile( buffer.GetString(), file_path );
		}
	}
}
