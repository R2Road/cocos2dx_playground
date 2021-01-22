#include "algorithm_practice_floodfill_Grid4Floodfill.h"

#include <fstream>

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace algorithm_practice_floodfill
{
	Grid4Floodfill::Grid4Floodfill( const std::size_t width, const std::size_t height ) : cpg::Grid<GridValue>( width, height )
	{}

	void Grid4Floodfill::ExportJsonString( std::string& out_json_string ) const
	{
		rapidjson::Document document;
		document.SetArray();
		for( const auto& v : GetContainer() )
		{
			document.PushBack( static_cast<int>( v.Type ), document.GetAllocator() );
		}

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
		document.Accept( writer );

		out_json_string = buffer.GetString();
	}
	bool Grid4Floodfill::LoadJsonString( const std::string& json_string )
	{
		rapidjson::Document doc;
		doc.Parse<0>( json_string.c_str() );

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

			Set( cur, { static_cast<eGridType>( value.GetInt() ) } );
		}

		return true;
	}
}
