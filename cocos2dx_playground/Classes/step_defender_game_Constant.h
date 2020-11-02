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

		const struct TileSheetConfiguration
		{
			int TileWidth = 32;
			int TileHeight = 32;

			int TileMargin_Width = 1;
			int TileMargin_Height = 1;

			std::string TexturePath = "textures/step_defender/step_defender_tilesheet_01.png";
		} TileSheetConfig;
	}
}
