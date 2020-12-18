#pragma once

#include <vector>

#include "2d/CCNode.h"

namespace step_flipflip
{
	namespace game
	{
		class CardViewNode;

		struct StageConfig;

		class StageViewNode : public cocos2d::Node
		{
		private:
			using CardViewContainerT = std::vector<CardViewNode>;

			StageViewNode();

		public:
			static StageViewNode* create( const StageConfig& stage_config, const bool show_guide = false );

		private:
			bool init( const StageConfig& stage_config, const bool show_guide );

		private:
			CardViewContainerT mCardViewContainer;
		};
	}
}
