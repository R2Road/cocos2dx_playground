#pragma once

#include "base/ccTypes.h"
#include "math/CCGeometry.h"

namespace step_defender
{
	namespace game
	{
		const struct
		{
			cocos2d::Size WorldSize = cocos2d::Size( 960.f, 320.f );
			cocos2d::Color4B SkyColor = cocos2d::Color4B::BLACK;
			cocos2d::Size BottomSize = cocos2d::Size( 960.f, 80.f );
		} WorldConfig;

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

			const int TileWidth;
			const int TileHeight;

			const int TileMargin_Width;
			const int TileMargin_Height;

			const int BlockWidth;
			const int BlockHeight;

			const std::string TexturePath;
		};
		const TileSheetConfiguration TileSheetConfig( 32, 32, 1, 1, "textures/step_defender/step_defender_tilesheet_01.png" );
	}
}
