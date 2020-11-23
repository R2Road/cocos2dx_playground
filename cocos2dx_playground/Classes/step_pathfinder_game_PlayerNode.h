#pragma once

#include "2d/CCNode.h"

namespace step_pathfinder
{
	namespace game
	{
		class PlayerNode : public cocos2d::Node
		{
		private:
			PlayerNode();

		public:
			static PlayerNode* create();

		private:
			bool init() override;
		};
	}
}
