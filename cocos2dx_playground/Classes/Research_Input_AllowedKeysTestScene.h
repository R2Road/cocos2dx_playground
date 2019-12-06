#pragma once

#include "cocos2d.h"

namespace Research
{
	namespace Input
	{
		class AllowedKeysTestScene : public cocos2d::Scene
		{
		public:
			AllowedKeysTestScene();

			static cocos2d::Scene* create();

			bool init() override;
		};
	}
}
