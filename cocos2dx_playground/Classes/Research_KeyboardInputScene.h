#pragma once

#include <vector>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
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
		CPG::Input::KeyMapSp key_map;
		CPG::Input::AnalyzerSp input_analyzer;

		struct ArrowData
		{
			ArrowData( int _key_index, cocos2d::Sprite* _sprite ) : key_index( _key_index ), sprite( _sprite ) {}

			int key_index;
			cocos2d::Sprite* sprite;
		};
		std::vector<ArrowData> arrow_views;
	};
}
