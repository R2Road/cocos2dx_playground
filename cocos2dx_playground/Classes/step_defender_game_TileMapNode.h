#pragma once

#include <string>

#include "2d/CCNode.h"

#include "step_defender_game_Constant.h"
#include "step_defender_game_TileSheetUtility.h"

NS_CC_BEGIN
class Sprite;
class SpriteBatchNode;
NS_CC_END

namespace step_defender
{
	namespace game
	{
		class TileMapNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				int MapWidth = 10;
				int MapHeight = 10;
			};

		private:
			TileMapNode( const Config& config, const TileSheetConfiguration& tile_sheet_config );

		public:
			~TileMapNode();

			static TileMapNode* create( const Config& config, const TileSheetConfiguration& tile_sheet_config );

		private:
			bool init() override;

		public:
			void Reset();

		private:
			const Config mConfig;
			const TileSheetConfiguration mTileSheetConfig;
			TileSheetUtility mTileSheetUtility;

			cocos2d::SpriteBatchNode* mSpriteBatchNode;
			cocos2d::Sprite* mReusedSprite;
		};
	}
}
