#pragma once

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;
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
			CPG::Input::KeyCollectorSp input_collector;
		};
	}
}
