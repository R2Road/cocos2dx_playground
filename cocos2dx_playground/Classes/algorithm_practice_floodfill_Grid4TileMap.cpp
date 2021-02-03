#include "algorithm_practice_floodfill_Grid4TileMap.h"

#include <fstream>

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace algorithm_practice_floodfill
{
	Grid4TileMap::Grid4TileMap() : mEntryPoint( { 0, 0 } ), mGrid()
	{}

	void Grid4TileMap::Reset( const std::size_t new_width, const std::size_t new_height )
	{
		mGrid.Reset( new_width, new_height );
	}


	eCellType Grid4TileMap::GetCellType( const std::size_t x, const std::size_t y )
	{
		return mGrid.Get( x, y );
	}
	eCellType Grid4TileMap::GetCellType( const std::size_t x, const std::size_t y ) const
	{
		return mGrid.Get( x, y );
	}


	void Grid4TileMap::SetEntryPoint( const cpg::Point& new_entry_point )
	{
		CCASSERT(
			0 <= new_entry_point.x && static_cast<int>( mGrid.GetWidth() ) > new_entry_point.x
			&& 0 <= new_entry_point.y && static_cast<int>( mGrid.GetHeight() ) > new_entry_point.y
			, "Failed : Grid4TileMap::SetEntryPoint"
		);

		mEntryPoint = new_entry_point;
		mGrid.Set( mEntryPoint.x, mEntryPoint.y, eCellType::Road );
	}
	void Grid4TileMap::SetCellType( const std::size_t x, const std::size_t y, const eCellType cell_type )
	{
		mGrid.Set( x, y, cell_type );
	}


	void Grid4TileMap::ExportJsonString( std::string& out_json_string ) const
	{
		rapidjson::Document document;
		document.SetObject();

		//
		// Size
		//
		{
			document.AddMember( "width", mGrid.GetWidth(), document.GetAllocator() );
			document.AddMember( "height", mGrid.GetHeight(), document.GetAllocator() );
		}

		//
		// Grid
		//
		{
			rapidjson::Value grid_value;
			grid_value.SetArray();
			for( const auto& v : mGrid )
			{
				grid_value.PushBack( static_cast<int>( v ), document.GetAllocator() );
			}
			document.AddMember( "grid", grid_value, document.GetAllocator() );
		}

		//
		// Entry Point
		//
		{
			rapidjson::Value entry_point_value;
			entry_point_value.SetObject();

			entry_point_value.AddMember( "x", mEntryPoint.x, document.GetAllocator() );
			entry_point_value.AddMember( "y", mEntryPoint.y, document.GetAllocator() );

			document.AddMember( "entry", entry_point_value, document.GetAllocator() );
		}

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
		document.Accept( writer );

		out_json_string = buffer.GetString();
		out_json_string += "\n";
	}
	bool Grid4TileMap::LoadJsonString( const std::string& json_string )
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

		if( !doc.IsObject() )
		{
			CCLOG( "invalid data struct" );
			return false;
		}

		//
		// Size
		//
		{
			int width = 1;
			{
				const auto temp_itr = doc.FindMember( "width" );
				CCASSERT( doc.MemberEnd() != temp_itr, "property not found" );
				width = temp_itr->value.GetInt();
			}

			int height = 1;
			{
				const auto temp_itr = doc.FindMember( "height" );
				CCASSERT( doc.MemberEnd() != temp_itr, "property not found" );
				height = temp_itr->value.GetInt();
			}

			mGrid.Reset( width, height );
		}

		//
		// Grid
		//
		{
			const auto grid_itr = doc.FindMember( "grid" );
			CCASSERT( doc.MemberEnd() != grid_itr, "property not found" );

			for( rapidjson::SizeType cur = 0u, end = grid_itr->value.Size(); cur < end; ++cur )
			{
				const auto& value = grid_itr->value[cur];

				mGrid.Set( cur, { static_cast<eCellType>( value.GetInt() ) } );
			}
		}

		//
		// Entry Point
		//
		{
			const auto temp_itr = doc.FindMember( "entry" );
			CCASSERT( doc.MemberEnd() != temp_itr, "property not found" );

			const auto entry_point_x = temp_itr->value["x"].GetInt();
			const auto entry_point_y = temp_itr->value["y"].GetInt();

			SetEntryPoint( cpg::Point{ entry_point_x, entry_point_y } );
		}

		return true;
	}
}
