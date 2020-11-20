#include "step_pathfinder_game_TerrainViewer.h"

#include <new>

#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"

#include "step_pathfinder_game_TerrainData.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace game
	{
		TerrainViewer::TerrainViewer( const int width, const int height, const cocos2d::Size tile_size, const float tile_scale ) :
			mWidth( width )
			, mHeight( height )
			, mTileSize( tile_size )
			, mTileScale( tile_scale )
			, mGridIndexConverter( width, height )
		{}

		TerrainViewer* TerrainViewer::create( const int width, const int height, const cocos2d::Size tile_size )
		{
			const float tile_scale = CalculateTileScale( tile_size.height );

			auto ret = new ( std::nothrow ) TerrainViewer( width, height, tile_size, tile_scale );
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool TerrainViewer::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			setContentSize( Size( mTileSize.width * mWidth, mTileSize.height * mHeight ) );

			const auto& tile_data = TileType2TileData( eTileType::road );
			Node* tile_node = nullptr;
			for( int grid_y = 0; grid_y < mHeight; ++grid_y )
			{
				for( int grid_x = 0; grid_x < mWidth; ++grid_x )
				{
					tile_node = Sprite::createWithSpriteFrameName( tile_data.ResourcePath );
					tile_node->setTag( mGridIndexConverter.To_Linear( grid_x, grid_y) );
					tile_node->setScale( _director->getContentScaleFactor() * mTileScale );
					tile_node->setPosition( ConvertPoint2Position( grid_x, grid_y ) );
					addChild( tile_node );
				}
			}

			return true;
		}

		void TerrainViewer::LoadTerrainData4Game( const TerrainData& terrain_data )
		{
			eTileType tile_type;
			for( int ty = 0; ty < terrain_data.GetHeight(); ++ty )
			{
				for( int tx = 0; tx < terrain_data.GetWidth(); ++tx )
				{
					tile_type = terrain_data.Get( tx, ty );
					if( eTileType::exit == tile_type )
					{
						UpdateTile( tx, ty, eTileType::road );
					}
					else
					{
						UpdateTile( tx, ty, terrain_data.Get( tx, ty ) );
					}
				}
			}
		}
		void TerrainViewer::LoadTerrainData4Original( const TerrainData& terrain_data )
		{
			for( int ty = 0; ty < terrain_data.GetHeight(); ++ty )
			{
				for( int tx = 0; tx < terrain_data.GetWidth(); ++tx )
				{
					UpdateTile( tx, ty, terrain_data.Get( tx, ty ) );
				}
			}
		}

		void TerrainViewer::UpdateTile( const int grid_x, const int grid_y, const eTileType tile_type )
		{
			auto tile_node = static_cast<Sprite*>( getChildByTag( mGridIndexConverter.To_Linear( grid_x, grid_y ) ) );
			if( !tile_node )
			{
				return;
			}

			const auto& tile_data = TileType2TileData( tile_type );
			tile_node->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath ) );
		}

		Vec2 TerrainViewer::ConvertPoint2Position( const int grid_x, const int grid_y ) const
		{
			return Vec2( mTileSize.width * 0.5f, mTileSize.height * 0.5f ) + Vec2( grid_x * mTileSize.width, grid_y * mTileSize.height );
		}
	}
}
