#pragma once

#include <vector>

#include "2d/CCNode.h"

#include "step_flipflip_game_Constant.h"

namespace step_flipflip
{
	namespace game
	{
		class CardViewNode;

		class StageViewNode : public cocos2d::Node
		{
		private:
			using CardViewContainerT = std::vector<CardViewNode>;

			StageViewNode();

		public:
			static StageViewNode* create( const int width, const int height, const bool show_guide = false );

		private:
			bool init( const int width, const int height, const bool show_guide );

		private:
			CardViewContainerT mCardViewContainer;
		};
	}
}
