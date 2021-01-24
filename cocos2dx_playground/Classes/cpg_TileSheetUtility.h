#pragma once

#include "math/CCGeometry.h"

namespace cpg
{
	class TileSheetUtility
	{
	public:
		TileSheetUtility();

		void Setup( const int tile_width, const int tile_height, const int tile_margin_width, const int tile_margin_height, const int tile_sheet_height );

		cocos2d::Rect ConvertTilePoint2TextureRect( const int x, const int y ) const;

	private:
		int TileWidth;
		int TileHeight;

		int TileMargin_Width;
		int TileMargin_Height;

		int BlockWidth;
		int BlockHeight;

		int TileSheetHeight;
	};
}
