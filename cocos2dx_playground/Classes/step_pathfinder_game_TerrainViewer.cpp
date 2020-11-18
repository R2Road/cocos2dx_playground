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
		{}

		float TerrainViewer::CalculateTileScale( const float tile_height )
		{
			const auto& tile_data = TileType2TileData( eTileType::road );
			return tile_height / ( SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath )->getRect().size.height * Director::getInstance()->getContentScaleFactor() );
		}

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
					tile_node = MakeTile( tile_data, tx, ty );
					tile_node->setPosition( ConvertPoint2Position( tx, ty ) );
					addChild( tile_node );
				}
			}

			return true;
		}

		Node* TerrainViewer::MakeTile( const TileData& tile_data, const int grid_x, const int grid_y )
		{
			const int linear_index = grid_x + ( mHeight * grid_y );

			auto tile_node = Sprite::createWithSpriteFrameName( tile_data.ResourcePath );
			tile_node->setTag( linear_index );
			tile_node->setScale( _director->getContentScaleFactor() * mTileScale );
			return tile_node;
		}
		void TerrainViewer::UpdateTile( Node* tile_node, const eTileType tile_type )
		{
			auto sprite = static_cast<Sprite*>( tile_node );

			const auto& tile_data = TileType2TileData( tile_type );
			sprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath ) );
		}
		void TerrainViewer::UpdateTile( const int grid_x, const int grid_y, const eTileType tile_type )
		{
			const int linear_index = grid_x + ( mWidth * grid_y );

			auto tile_node = getChildByTag( linear_index );
			if( !tile_node )
			{
				return;
			}

			UpdateTile( tile_node, tile_type );
		}

		cocos2d::Vec2 TerrainViewer::ConvertPoint2Position( const int tx, const int ty ) const
		{
			return Vec2( mTileSize.width * 0.5f, mTileSize.height * 0.5f ) + Vec2( tx * mTileSize.width, ty * mTileSize.height );
		}
	}
}
