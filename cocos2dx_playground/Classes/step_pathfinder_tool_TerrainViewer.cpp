#include "step_pathfinder_tool_TerrainViewer.h"

#include <new>

#include "2d/CCSpriteFrameCache.h"
#include "ui/UIButton.h"

#include "step_pathfinder_tool_TerrainData.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace tool
	{
		TerrainViewer::TerrainViewer(
			const int width, const int height
			, const cocos2d::Size tile_size
			, const cocos2d::Vec2 pivot_position
			, const TileSelectCallback& tile_select_callback
		) :
			game::terrain::Viewer( width, height, tile_size, pivot_position )
			, mTileSelectCallback( tile_select_callback )
		{}

		TerrainViewer* TerrainViewer::create( const int width, const int height, const TileSelectCallback& tile_select_callback )
		{
			const auto& tile_data = step_pathfinder::game::terrain::TileType2TileData( step_pathfinder::game::terrain::eTileType::road );
			const auto tile_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath )->getRect().size;
			const Vec2 pivot_position( tile_size.width * 0.5f, tile_size.height * 0.5f );

			auto ret = new ( std::nothrow ) TerrainViewer( width, height, tile_size, pivot_position, tile_select_callback );
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

		Node* TerrainViewer::MakeTile( const step_pathfinder::game::terrain::TileData& tile_data, const int grid_x, const int grid_y )
		{
			auto tile_node = game::terrain::Viewer::MakeTile( tile_data, grid_x, grid_y );
			{
				auto button = ui::Button::create( "guide_empty.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				button->setTag( tile_node->getTag() );
				button->setScale9Enabled( true );
				button->setContentSize( tile_node->getContentSize() );
				button->setPosition( Vec2( button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f ) );
				button->addTouchEventListener( mTileSelectCallback );
				tile_node->addChild( button );
			}

			return tile_node;
		}
	} // namespace tool
}
