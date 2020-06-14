#include "step_pathfinder_game_terrain_Viewer.h"

#include <new>

#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"

#include "step_pathfinder_game_TerrainData.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace game
	{
		Viewer::Viewer( const int width, const int height, const cocos2d::Size tile_size, const cocos2d::Vec2 pivot_position ) :
			mWidth( width )
			, mHeight( height )
			, mTileSize( tile_size )
			, mPivotPosition( pivot_position )
		{}

		Viewer* Viewer::create( const int width, const int height )
		{
			const auto& tile_data = TileType2TileData( eTileType::road );
			const auto tile_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath )->getRect().size;
			const Vec2 pivot_position( tile_size.width * 0.5f, tile_size.height * 0.5f );

			auto ret = new ( std::nothrow ) Viewer( width, height, tile_size, pivot_position );
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool Viewer::init()
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

		Node* Viewer::MakeTile( const TileData& tile_data, const int grid_x, const int grid_y )
		{
			const int linear_index = grid_x + ( mHeight * grid_y );

			auto tile_node = Sprite::createWithSpriteFrameName( tile_data.ResourcePath );
			tile_node->setTag( linear_index );
			return tile_node;
		}
		void Viewer::UpdateTile( Node* tile_node, const eTileType tile_type )
		{
			auto sprite = static_cast<Sprite*>( tile_node );

			const auto& tile_data = TileType2TileData( tile_type );
			sprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath ) );
		}
		void Viewer::UpdateTile( const int grid_x, const int grid_y, const eTileType tile_type )
		{
			const int linear_index = grid_x + ( mWidth * grid_y );

			auto grid = getChildByTag( linear_index );
			if( !grid )
			{
				return;
			}

			UpdateTile( grid, tile_type );
		}

		cocos2d::Vec2 Viewer::ConvertPoint2Position( const int tx, const int ty ) const
		{
			return mPivotPosition + Vec2( ( tx * mTileSize.width ), ( ty * mTileSize.height ) );
		}
	}
}
