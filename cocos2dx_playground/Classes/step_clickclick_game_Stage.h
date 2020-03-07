#pragma once

#include "2d/CCNode.h"

namespace step_clickclick
{
	namespace game
	{
		class Stage : public cocos2d::Node
		{
		private:
			Stage() = default;

		public:
			static Stage* create();

			bool init() override;
		};
	}
}
