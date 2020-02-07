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
		const int map_size_width = 5;
		const int map_size_height = 5;

		TerrainData::TerrainData() :
			mWidth( map_size_width )
			, mHeight( map_size_height )
			, mContainer()
		{
			mContainer.resize( mHeight );
			for( auto& r : mContainer )
			{
				r.resize( mWidth, step01::game::terrain::eTileType::damage );
			}
		}

		void TerrainData::fill( const Row::value_type tile_type )
		{
			for( auto& r : mContainer )
			{
				for( auto& t : r )
				{
					t = tile_type;
				}
			}
		}

		TerrainData::Row::value_type TerrainData::get( const std::size_t x, const std::size_t y ) const
		{
			if( mContainer.size() <= y )
			{
				return step01::game::terrain::eTileType::damage;
			}

			if( mContainer[y].size() <= x )
			{
				return step01::game::terrain::eTileType::damage;
			}

			return mContainer[y][x];
		}
		void TerrainData::set( const std::size_t x, const std::size_t y, const Row::value_type tile_type )
		{
			if( mContainer.size() <= y )
			{
				return;
			}

			if( mContainer[y].size() <= x )
			{
				return;
			}

			mContainer[y][x] = tile_type;
		}

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

			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( file_name );
			path.append( ".stage" );
			std::ofstream fs( path, std::ios::out );
			fs << buffer.GetString() << std::endl;
			fs.close();
		}

		bool TerrainData::load( const char* file_name )
		{
			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( file_name );
			path.append( ".stage" );

			const std::string regionStr( std::move( cocos2d::FileUtils::getInstance()->getStringFromFile( path ) ) );
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
