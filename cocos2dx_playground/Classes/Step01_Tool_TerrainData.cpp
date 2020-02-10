#include "Step01_Tool_TerrainData.h"

#include <fstream>

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace step01
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

		bool TerrainData::load( const char* file_name )
		{
			const std::string regionStr( std::move( cocos2d::FileUtils::getInstance()->getStringFromFile( file_name ) ) );
			rapidjson::Document doc;
			doc.Parse<0>( regionStr.c_str() );

			if( doc.HasParseError() )
			{
				CCLOG( "json parse error" );
				return false;
			}

			if( doc.IsNull() )
			{
				CCLOG( "json is empty" );
				return false;
			}

			if( !doc.IsArray() )
			{
				CCLOG( "invalid data struct" );
				return false;
			}

			int x = 0;
			int y = 0;
			for( rapidjson::SizeType cur = 0u, end = doc.Size(); cur < end; ++cur )
			{
				const auto& value = doc[cur];

				y = cur / mHeight;
				x = cur - ( y * mWidth );

				set( x, y, static_cast<step01::game::terrain::eTileType>( value.GetInt() ) );
			}

			return true;
		}
	}
}
