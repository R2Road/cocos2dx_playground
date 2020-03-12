#pragma once

#include <memory>
#include <vector>

#include "cpg_GridIndexConverter.h"
#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		using StageUp = std::unique_ptr<class Stage>;
		class Stage
		{
		public:
			class Block
			{
			public:
				Block( const int index, const int life );

				void Init( eBlockType type, const int life );
				void DecreaseAction();
				void IncreaseAction();
				void DieAction();

				int GetIndex() const { return mIndex; }
				eBlockType GetType() const { return mBlockType; }
				bool IsActive() const { return mActive; }
				int GetLife() const { return mLife; }

			private:
				int mIndex;
				eBlockType mBlockType;
				bool mActive;
				int mLife;
			};

		private:
			Stage( const int width, const int height );

		public:
			static StageUp create( const int width, const int height );

			bool init();

			void Setup( const int width, const int height );

			int GetWidth() const { return mStageWidth; }
			int GetHeight() const { return mStageHeight; }
			const std::vector<Block>& GetBlockDatas() const { return mBlocks; }
			const Block& GetBlockData( const int linear_index ) const;
			void IncreaseBlockLife( const int linear_index );
			void DecreaseBlockLife( const int linear_index );
			void DieBlock( const int linear_index );

			bool HasActiveBlock() const;

		private:
			const int mStageWidth;
			const int mStageHeight;
			const int mCenterX;
			const int mCenterY;
			const cpg::GridIndexConverter mGridIndexConverter;
			std::vector<Block> mBlocks;
		};
	}
}
