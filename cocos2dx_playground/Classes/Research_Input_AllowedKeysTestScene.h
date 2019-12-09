#pragma once

#include <string>
#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		class Delegator;
	}

	namespace InputTest
	{
		class KeyboardInputObserber;
	}
}

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
			void update( float dt ) override;

		private:
			CPG::Input::Delegator* input_delegator;
			cocos2d::Label* key_viewer;
			std::string key_string;

			CPG::InputTest::KeyboardInputObserber* input_obserber;
		};
	}
}
