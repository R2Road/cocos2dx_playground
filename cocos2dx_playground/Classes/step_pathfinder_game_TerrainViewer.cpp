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
			for( int ty = 0; ty < mHeight; ++ty )
			{
				for( int tx = 0; tx < mWidth; ++tx )
				{
					tile_node = Sprite::createWithSpriteFrameName( tile_data.ResourcePath );
					tile_node->setTag( mGridIndexConverter.To_Linear( tx, ty) );
					tile_node->setScale( _director->getContentScaleFactor() * mTileScale );
					tile_node->setPosition( ConvertPoint2Position( tx, ty ) );
					addChild( tile_node );
				}
			}

			return true;
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
