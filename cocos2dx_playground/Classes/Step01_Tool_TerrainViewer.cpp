#include "Step01_Tool_TerrainViewer.h"

#include <new>

#include "cocos2d.h"
#include "ui/UIButton.h"

#include "Step01_Tool_TerrainData.h"

USING_NS_CC;

namespace step01
{
	namespace tool
	{
		TerrainViewer::TerrainViewer( const int width, const int height, const TileSelectCallback& tile_select_callback ) :
			mWidth( width )
			, mHeight( height )
			, mTileSelectCallback( tile_select_callback )
		{}

		TerrainViewer* TerrainViewer::create( const int width, const int height, const TileSelectCallback& tile_select_callback )
		{
			auto ret = new ( std::nothrow ) TerrainViewer( width, height, tile_select_callback );
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

		bool TerrainViewer::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			const auto tile_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( "guide_01_1.png" )->getRect().size;
			setContentSize( Size( tile_size.width * mWidth, tile_size.height * mHeight ) );

			const auto& tile_data = step01::game::terrain::TileType2TileData( step01::game::terrain::eTileType::road );
			const Vec2 pivot_position( tile_size.width * 0.5f, tile_size.height * 0.5f );;

			Node* button = nullptr;
			for( int ty = 0; ty < mHeight; ++ty )
			{
				for( int tx = 0; tx < mWidth; ++tx )
				{
					button = MakeTile( tile_data, tx, ty );
					button->setPosition( pivot_position + Vec2( ( tx * tile_size.width ), ( ty * tile_size.height ) ) );
					addChild( button );
				}
			}

			return true;
		}

		Node* TerrainViewer::MakeTile( const step01::game::terrain::TileData& tile_data, const int grid_x, const int grid_y )
		{
			const int linear_index = grid_x + ( mHeight * grid_y );

			auto indicator = Sprite::createWithSpriteFrameName( tile_data.ResourcePath );
			indicator->setTag( linear_index );
			{
				auto button = ui::Button::create( "guide_empty.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				button->setTag( linear_index );
				button->setScale9Enabled( true );
				button->setContentSize( indicator->getContentSize() );
				button->setPosition( Vec2( button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f ) );
				button->addTouchEventListener( mTileSelectCallback );
				indicator->addChild( button );
			}

			return indicator;
		}
		void TerrainViewer::UpdateTile( Node* tile_node, const step01::game::terrain::eTileType tile_type )
		{
			auto indicator = static_cast<Sprite*>( tile_node );
			if( !indicator )
			{
				return;
			}

			const auto& tile_data = step01::game::terrain::TileType2TileData( tile_type );
			indicator->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath ) );
		}
		void TerrainViewer::UpdateTile( const int grid_x, const int grid_y, const step01::game::terrain::eTileType tile_type )
		{
			const int linear_index = grid_x + ( mWidth * grid_y );

			auto grid = getChildByTag( linear_index );
			if( !grid )
			{
				return;
			}

			UpdateTile( grid, tile_type );
		}
	} // namespace tool
}
