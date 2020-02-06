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
		TerrainData::TerrainData() :
			mWidth( 0 )
			, mHeight( 0 )
			, mContainer()
		{}

		void TerrainData::reSize( const std::size_t width, const std::size_t height )
		{
			mWidth = width;
			mHeight = height;
			mContainer.resize( height );
			for( auto& r : mContainer )
			{
				r.resize( width, step01::game::terrain::eTileType::damage );
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
			document.SetObject();

			//
			// sizse info
			//
			{
				rapidjson::Value size_info_value;
				size_info_value.SetObject();

				size_info_value.AddMember( "w", mWidth, document.GetAllocator() );
				size_info_value.AddMember( "h", mWidth, document.GetAllocator() );

				document.AddMember( "size", size_info_value, document.GetAllocator() );
			}

			//
			// terrain info
			//
			{
				rapidjson::Value terrain_info_value;
				terrain_info_value.SetArray();

				for( auto& row : mContainer )
				{
					for( auto& t : row )
					{
						terrain_info_value.PushBack( static_cast<int>( t ), document.GetAllocator() );
					}
				}

				document.AddMember( "terrain", terrain_info_value, document.GetAllocator() );
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

			if( !doc.IsObject() )
			{
				CCLOG( "invalid data struct" );
				return false;
			}

			rapidjson::Value::MemberIterator temp_itr;
			const auto size_itr = doc.FindMember( "size" );
			if( doc.MemberEnd() == size_itr )
			{
				CCLOG( "Size nothing" );
				return false;
			}
			else
			{
				temp_itr = size_itr->value.FindMember( "w" );
				if( size_itr->value.MemberEnd() == temp_itr )
				{
					CCLOG( "Invalid Size Value : w" );
					return false;
				}
				mWidth = temp_itr->value.GetInt();
				
				temp_itr = size_itr->value.FindMember( "h" );
				if( size_itr->value.MemberEnd() == temp_itr )
				{
					CCLOG( "Invalid Size Value : h" );
					return false;
				}
				mHeight = temp_itr->value.GetInt();
			}

			const auto terrain_itr = doc.FindMember( "terrain" );
			if( doc.MemberEnd() == terrain_itr )
			{
				CCLOG( "Terrain nothing" );
				return false;
			}
			else
			{
				reSize( mWidth, mHeight );
				int x = 0;
				int y = 0;
				for( rapidjson::SizeType cur = 0u, end = terrain_itr->value.Size(); cur < end; ++cur )
				{
					const auto& value = terrain_itr->value[cur];

					y = cur / mHeight;
					x = cur - ( y * mWidth );

					set( x, y, static_cast<step01::game::terrain::eTileType>( value.GetInt() ) );
				}
			}
		}
	}
}
