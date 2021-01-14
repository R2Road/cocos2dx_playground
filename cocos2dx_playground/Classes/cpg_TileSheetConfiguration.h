#pragma once

#include <string>

namespace cpg
{
	struct TileSheetConfiguration
	{
		TileSheetConfiguration( int tile_width, int tile_height, int tile_margin_width, int tile_margin_height, const char* texture_path ) :
			TileWidth( tile_width )
			, TileHeight( tile_height )

			, TileMargin_Width( tile_margin_width )
			, TileMargin_Height( tile_margin_height )

			, BlockWidth( TileMargin_Width + TileWidth +TileMargin_Width )
			, BlockHeight( TileMargin_Height + TileHeight +TileMargin_Height )

			, TexturePath( texture_path )
		{}

		int TileWidth;
		int TileHeight;

		int TileMargin_Width;
		int TileMargin_Height;

		int BlockWidth;
		int BlockHeight;

		std::string TexturePath;
	};
}
