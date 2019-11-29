#pragma once

#include <list>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		using AnalyzerSp = std::shared_ptr<class Analyzer>;

		class KeyViewer;
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
		std::list<CPG::Input::KeyViewer*> key_viewer_list;
		cocos2d::Vec2 key_viewer_start_position;
		cocos2d::Vec2 key_viewer_end_position;
	};
}
