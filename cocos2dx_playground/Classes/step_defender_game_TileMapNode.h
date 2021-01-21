#pragma once

#include <string>

#include "2d/CCSpriteBatchNode.h"

#include "step_defender_game_Constant.h"
#include "cpg_TileSheetUtility.h"

NS_CC_BEGIN
class Sprite;
NS_CC_END

namespace step_defender
{
	namespace game
	{
		class TileMapNode : public cocos2d::SpriteBatchNode
		{
		public:
			struct Config
			{
				int MapWidth = 10;
				int MapHeight = 10;
			};

		private:
			TileMapNode( const Config& config, const cpg::TileSheetConfiguration& tile_sheet_config );

		public:
			~TileMapNode();

			static TileMapNode* create( const Config& config, const cpg::TileSheetConfiguration& tile_sheet_config );

		private:
			bool init() override;

			void updateColor() override;

		public:
			void Reset();
			void FillAll( const int tile_point_x, const int tile_point_y );
			void UpdateTile( const int map_point_x, const int map_point_y, const int tile_point_x, const int tile_point_y );
			void EraseTile( const int map_point_x, const int map_point_y );

		private:
			const Config mConfig;
			const cpg::TileSheetConfiguration mTileSheetConfig;
			cpg::TileSheetUtility mTileSheetUtility;

			cocos2d::Sprite* mReusedSprite;
		};
	}
}
