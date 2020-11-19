#include "step_pathfinder_tool_TerrainEditHelper.h"

#include <new>

#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "step_pathfinder_tool_TerrainData.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace tool
	{
		TerrainEditHelper::TerrainEditHelper(
			const int width, const int height
			, const cocos2d::Size tile_size
			, const float tile_scale
			, const TileSelectCallback& tile_select_callback
		) :
			game::TerrainViewer( width, height, tile_size, tile_scale )
			, mTileSelectCallback( tile_select_callback )
		{}

		TerrainEditHelper* TerrainEditHelper::create( const int width, const int height, const cocos2d::Size tile_size, const TileSelectCallback& tile_select_callback )
		{
			const float tile_scale = CalculateTileScale( tile_size.height );

			auto ret = new ( std::nothrow ) TerrainEditHelper( width, height, tile_size, tile_scale, tile_select_callback );
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

		Node* TerrainEditHelper::MakeTile( const step_pathfinder::game::TileData& tile_data, const int grid_x, const int grid_y )
		{
			auto tile_node = game::TerrainViewer::MakeTile( tile_data, grid_x, grid_y );
			{
				auto button = ui::Button::create( "guide_01_3.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setTag( tile_node->getTag() );
				button->setScale9Enabled( true );
				button->setContentSize( tile_node->getContentSize() );
				button->getRendererNormal()->setOpacity( 100u );
				button->setPosition( Vec2( button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f ) );
				button->addTouchEventListener( mTileSelectCallback );
				tile_node->addChild( button );
			}

			return tile_node;
		}
	} // namespace tool
}
