#pragma once

#include <functional>
#include <vector>

#include "2d/CCNode.h"

#include "cpg_GridIndexConverter.h"

NS_CC_BEGIN
	class Label;
	class Sprite;
	class Action;
NS_CC_END

namespace step_clickclick
{
	namespace game
	{
		class BlockViewNode;

		class Stage;

		class StageViewNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				bool bShowPivot = false;
				bool bShowBackgroundGuide = false;
			};

			using OnBlockCallback = std::function<void( int )>;

		private:
			StageViewNode( const int width, const int height );

		public:
			static StageViewNode* create( const Config config, const int width, const int height, const OnBlockCallback& on_block_callback );

		private:
			bool init( const Config config, const OnBlockCallback& on_block_callback );

		public:
			void Setup( const Stage& stage_data );
			void UpdateBlock( const int linear_index, const int current_life );

		private:
			const int mStageWidth;
			const int mStageHeight;
			const cpg::GridIndexConverter mGridIndexConverter;
			std::vector<BlockViewNode*> mBlockViews;
		};
	}
}
