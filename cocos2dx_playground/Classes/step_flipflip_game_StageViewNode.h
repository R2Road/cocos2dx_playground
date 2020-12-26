#pragma once

#include <vector>

#include "2d/CCNode.h"

#include "cpg_GridIndexConverter.h"

namespace step_flipflip
{
	namespace game
	{
		class CardViewNode;
		struct StageConfig;
		class StageData;

		class StageViewNode : public cocos2d::Node
		{
		private:
			using CardViewContainerT = std::vector<CardViewNode*>;

			StageViewNode();

		public:
			static StageViewNode* create( const StageConfig& stage_config, const StageData& stage_data, const bool show_guide = false );

		private:
			bool init( const StageConfig& stage_config, const StageData& stage_data, const bool show_guide );

		public:
			void HideAll();
			void Flip( const int x, const int y );
			bool isFlipping();

		private:
			cpg::GridIndexConverter mIndexConverter;
			CardViewContainerT mCardViewContainer;

			int mLastFlipedX;
			int mLastFlipedY;
		};
	}
}
