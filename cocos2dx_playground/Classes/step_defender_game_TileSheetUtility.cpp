#include "step_defender_game_TileSheetUtility.h"

#include <algorithm>

#include "base/ccMacros.h"
#include "base/CCDirector.h" // for CC_RECT_PIXELS_TO_POINTS

namespace step_defender
{
	namespace game
	{
		TileSheetUtility::TileSheetUtility() :
			TileWidth( 1 )
			, TileHeight( 1 )

			, TileMargin_Width( 0 )
			, TileMargin_Height( 0 )

			, BlockWidth( TileMargin_Width + TileWidth + TileMargin_Width )
			, BlockHeight( TileMargin_Height + TileHeight + TileMargin_Height )

			, TileSheetHeight( 1 )
		{}

		void TileSheetUtility::Setup( const int tile_width, const int tile_height, const int tile_margin_width, const int tile_margin_height, const int tile_sheet_height )
		{
			TileWidth = tile_width;
			TileHeight = tile_height;

			TileMargin_Width = tile_margin_width;
			TileMargin_Height = tile_margin_height;

			BlockWidth = TileMargin_Width + TileWidth + TileMargin_Width;
			BlockHeight = TileMargin_Height + TileHeight + TileMargin_Height;

			TileSheetHeight = tile_sheet_height;
		}

		cocos2d::Rect TileSheetUtility::ConvertTilePoint2TextureRect( const int x, const int y ) const
		{
			cocos2d::Rect temp_rect(
				( x * BlockWidth ) + TileMargin_Width
				, ( TileSheetHeight - ( y * BlockHeight ) - BlockHeight ) + TileMargin_Height
				, TileWidth, TileHeight
			);
			//CCLOG( "Orig %f, %f", temp_rect.origin.x, temp_rect.origin.y );

			temp_rect = CC_RECT_PIXELS_TO_POINTS( temp_rect );
			//CCLOG( "Fixed %f, %f", temp_rect.origin.x, temp_rect.origin.y );

			return temp_rect;
		}
	}
}
