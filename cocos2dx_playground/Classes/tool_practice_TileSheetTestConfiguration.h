#pragma once

#include "step_defender_game_Constant.h"

namespace tool_practice
{
	class TileSheetTestConfiguration
	{
	public:
		TileSheetTestConfiguration();

		bool Load( const char* config_file_path );

		int GetLayerCount() const { return mLayerCount; }
		int GetWidth() const { return mWidth; }
		int GetHeight() const { return mHeight; }
		const cpg::TileSheetConfiguration& GetTileSheetConfiguration() const { return mTileSheetConfiguration; }

	private:
		int mLayerCount;
		int mWidth;
		int mHeight;
		cpg::TileSheetConfiguration mTileSheetConfiguration;
	};
}
