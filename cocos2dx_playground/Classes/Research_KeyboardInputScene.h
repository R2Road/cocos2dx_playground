#pragma once

#include <array>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using AnalyzerSp = std::shared_ptr<class Analyzer>;
	}
}

namespace Research
{
	class KeyboardInputScene : public cocos2d::Scene
	{
	public:
		KeyboardInputScene();

		static cocos2d::Scene* create();

		bool init() override;
		void update( float dt ) override;

	private:
		CPG::Input::AnalyzerSp input_analyzer;

		std::array<cocos2d::Sprite*, 4u> arrow_views;
	};
}
