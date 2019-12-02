#pragma once

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		using BasicCollectorSp = std::shared_ptr<class BasicCollector>;
	}
}

namespace Research
{
	namespace Input
	{
		class KeyConfigScene : public cocos2d::Scene
		{
		public:
			KeyConfigScene() = default;

			static cocos2d::Scene* create();

			void update( float dt ) override;

		private:
			CPG::Input::BasicCollectorSp input_collector;
		};
	}
}
