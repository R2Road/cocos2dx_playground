#pragma once

#include "base/ccTypes.h"
#include "math/CCGeometry.h"

#include "cpg_TileSheetConfiguration.h"

namespace step_defender
{
	namespace game
	{
		const struct
		{
			cocos2d::Size WorldSize = cocos2d::Size( 960.f, 320.f );
			cocos2d::Color4B SkyColor = cocos2d::Color4B( 95u, 205u, 228u, 255u );
			cocos2d::Size BottomSize = cocos2d::Size( 960.f, 80.f );
		} WorldConfig;

		const cpg::TileSheetConfiguration TileSheetConfig( 16, 16, 1, 1, "textures/step_defender/step_defender_tilesheet_01.png" );

		struct ParallaxNodeConfig
		{
			int Index = -1;
			float Rate = 1.f;
		};
		const ParallaxNodeConfig ParallaxNodeConfigContainer[3] = {
			{ 0, 0.2f }
			, { 1, 0.5f }
			, { 2, 1.f }
		};
	}
}
