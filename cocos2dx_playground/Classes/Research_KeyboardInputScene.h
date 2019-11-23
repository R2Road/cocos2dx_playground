#pragma once

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		class Delegator;
	}
}

namespace Research
{
	class KeyboardInputScene : public cocos2d::Scene
	{
	public:
		KeyboardInputScene();

		static cocos2d::Scene* create();

		void update( float dt ) override;

	private:
		CPG::Input::Delegator* input_delegator;
	};
}
