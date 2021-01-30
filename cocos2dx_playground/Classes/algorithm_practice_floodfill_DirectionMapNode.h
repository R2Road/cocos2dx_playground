#pragma once

#include <string>

#include "2d/CCSpriteBatchNode.h"

#include "algorithm_practice_floodfill_Constant.h"
#include "step_defender_game_Constant.h"
#include "cpg_TileSheetUtility.h"

NS_CC_BEGIN
class Sprite;
NS_CC_END

namespace algorithm_practice_floodfill
{
	class DirectionMapNode : public cocos2d::SpriteBatchNode
	{
	public:
		struct Config
		{
			int MapWidth = 10;
			int MapHeight = 10;
		};

	private:
		DirectionMapNode( const Config& config );

	public:
		~DirectionMapNode();

		static DirectionMapNode* create( const Config& config );

	private:
		bool init() override;
		void updateColor() override;

		std::size_t ConvertPoint2QuadIndex( const int point_x, const int point_y, const int offset ) const;
		cocos2d::Rect ConvertDirection2TileRect( const Direction4::eState direction_type ) const;

	public:
		void Reset();
		void UpdateTile( const int map_point_x, const int map_point_y, const char direction );
		void ClearTile( const int map_point_x, const int map_point_y );

	private:
		const Config mConfig;
		cpg::TileSheetConfiguration mTileSheetConfig;
		cpg::TileSheetUtility mTileSheetUtility;

		cocos2d::Sprite* mReusedSprite;
	};
}
