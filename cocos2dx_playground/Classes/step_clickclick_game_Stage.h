#pragma once

#include <memory>
#include <vector>

#include "cpg_GridIndexConverter.h"
#include "step_clickclick_game_Constant.h"
#include "step_clickclick_game_Block.h"

namespace step_clickclick
{
	namespace game
	{
		using StageUp = std::unique_ptr<class Stage>;
		class Stage
		{
		private:
			Stage( const int width, const int height );

		public:
			static StageUp create( const int width, const int height );

		private:
			void init();

		public:
			void Setup( const int width, const int height, const int shuffle_limit );

			int GetWidth() const { return mStageWidth; }
			int GetHeight() const { return mStageHeight; }
			bool isIn( const int x, const int y ) const;

			const std::vector<Block>& GetBlockDatas() const { return mBlocks; }
			const Block& GetBlockData( const int x, const int y ) const;
			const Block& GetBlockData( const int linear_index ) const;
			void IncreaseBlockLife( const int linear_index );
			void DecreaseBlockLife( const int linear_index );
			void DieBlock( const int linear_index );

			int GetActiveBlockCount() const { return mActiveBlockCount; }
			bool HasActiveBlock() const { return 0 != mActiveBlockCount; }

			cpg::Point ConvertLinearIndex2PointIndex( const int linear_index ) const
			{
				return mGridIndexConverter.To_Point( linear_index );
			}

		private:
			const int mStageWidth;
			const int mStageHeight;
			const int mCenterX;
			const int mCenterY;
			const cpg::GridIndexConverter mGridIndexConverter;
			std::vector<Block> mBlocks;
			int mActiveBlockCount;
		};
	}
}
