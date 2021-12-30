#pragma once

#include <string>

namespace cpg
{
	struct TileSheetConfiguration
	{
		TileSheetConfiguration() :
			TileWidth( 1 )
			, TileHeight( 1 )

			, TileMargin_Width( 1 )
			, TileMargin_Height( 1 )

			, BlockWidth( TileMargin_Width + TileWidth + TileMargin_Width )
			, BlockHeight( TileMargin_Height + TileHeight + TileMargin_Height )

			, TexturePath( "" )
		{}

		TileSheetConfiguration( int tile_width, int tile_height, int tile_margin_width, int tile_margin_height, const char* texture_path ) :
			TileWidth( tile_width )
			, TileHeight( tile_height )

			, TileMargin_Width( tile_margin_width )
			, TileMargin_Height( tile_margin_height )

			, BlockWidth( TileMargin_Width + TileWidth + TileMargin_Width )
			, BlockHeight( TileMargin_Height + TileHeight + TileMargin_Height )

			, TexturePath( texture_path )
		{}

		bool Load( const char* config_file_path );

		inline int GetTileWidth() const { return TileWidth; }
		inline int GetTileHeight() const { return TileHeight; }

		inline int GetTileMargin_Width() const { return TileMargin_Width; }
		inline int GetTileMargin_Height() const { return TileMargin_Height; }

		inline int GetBlockWidth() const { return BlockWidth; }
		inline int GetBlockHeight() const { return BlockHeight; }

		const std::string& GetTexturePath() const { return TexturePath; }

	private:
		int TileWidth;
		int TileHeight;

		int TileMargin_Width;
		int TileMargin_Height;

		int BlockWidth;
		int BlockHeight;

		std::string TexturePath;
	};
}
