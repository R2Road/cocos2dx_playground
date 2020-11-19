#include "step_pathfinder_tool_TerrainEditHelper.h"

#include <new>

#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "step_pathfinder_game_Constant.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace tool
	{
		TerrainEditHelper::TerrainEditHelper( const int width, const int height, const TileSelectCallback& tile_select_callback ) :
			mTileSelectCallback( tile_select_callback )
			, mGridIndexConverter( width, height )
		{}

		TerrainEditHelper* TerrainEditHelper::create( const int width, const int height, const Size tile_size, const TileSelectCallback& tile_select_callback )
		{
			auto ret = new ( std::nothrow ) TerrainEditHelper( width, height, tile_select_callback );
			if( !ret || !ret->init( width, height, tile_size ) )
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

		bool TerrainEditHelper::init( const int width, const int height, const Size tile_size )
		{
			if( !Node::init() )
			{
				return false;
			}

			setContentSize( Size( tile_size.width * width, tile_size.height * height ) );

			const auto& tile_data = game::TileType2TileData( game::eTileType::road );
			const auto tile_sprite_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( tile_data.ResourcePath );

			const auto tile_scale = game::CalculateTileScale( tile_size.height );
			const auto button_size = tile_sprite_frame->getOriginalSize() * _director->getContentScaleFactor() * tile_scale;

			ui::Button* button = nullptr;
			for( int ty = 0; ty < height; ++ty )
			{
				for( int tx = 0; tx < width; ++tx )
				{
					button = ui::Button::create( "guide_01_3.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->setTag( mGridIndexConverter.To_Linear( tx, ty ) );
					button->setAnchorPoint( Vec2::ZERO );
					button->setScale9Enabled( true );
					button->setContentSize( button_size );
					button->getRendererNormal()->setOpacity( 100u );
					button->setPosition( Vec2( tx * tile_size.width, ty * tile_size.height ) );
					button->addTouchEventListener( mTileSelectCallback );
					addChild( button );
				}
			}

			return true;
		}
	}
}
