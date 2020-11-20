#include "step_pathfinder_game_TerrainData.h"

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"

namespace step_pathfinder
{
	namespace game
	{
		TerrainData::TerrainData() :
			mWidth( 5 )
			, mHeight( 5 )
			, mContainer()
		{
			mContainer.resize( mHeight );
			for( auto& row : mContainer )
			{
				row.resize( mWidth, eTileType::damage );
			}
		}

		bool TerrainData::Load( const char* file_name )
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

				Set( x, y, static_cast<eTileType>( value.GetInt() ) );
			}

			return true;
		}

		TerrainData::ValueT TerrainData::Get( const int x, const int y ) const
		{
			if( !isIn( x, y ) )
			{
				return eTileType::damage;
			}

			return mContainer[y][x];
		}
		void TerrainData::Set( const int x, const int y, const ValueT tile_type )
		{
			if( !isIn( x, y ) )
			{
				return;
			}

			mContainer[y][x] = tile_type;
		}

		TerrainPoint TerrainData::FindTilePoint( const ValueT tile_type ) const
		{
			for( int y = 0; y < mHeight; ++y )
			{
				for( int x = 0; x < mWidth; ++x )
				{
					if( tile_type == mContainer[y][x] )
					{
						return TerrainPoint{ x, y };
					}
				}
			}

			return TerrainPoint{ -1, -1 };
		}
		bool TerrainData::isExist( const ValueT tile_type ) const
		{
			for( int y = 0; y < mHeight; ++y )
			{
				for( int x = 0; x < mWidth; ++x )
				{
					if( tile_type == mContainer[y][x] )
					{
						return true;
					}
				}
			}

			return false;
		}
		bool TerrainData::isIn( const int x, const int y ) const
		{
			if( 0 > y || static_cast<int>( mContainer.size() ) <= y )
			{
				return false;
			}

			if( 0 > x || static_cast<int>( mContainer[y].size() ) <= x )
			{
				return false;
			}

			return true;
		}
	} // namespace game
}
