#pragma once

#include <list>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;
	}

	namespace InputTest
	{
		class KeyViewer;
	}
}

namespace Research
{
	namespace Input
	{
		class KeyboardTestScene : public cocos2d::Scene
		{
		public:
			KeyboardTestScene();

			static cocos2d::Scene* create();

			bool init() override;
			void update( float dt ) override;

		private:
			CPG::Input::KeyMapSp key_map;
			CPG::Input::KeyCollectorSp input_collector;
			std::list<CPG::InputTest::KeyViewer*> key_viewer_list;
			cocos2d::Vec2 key_viewer_start_position;
			cocos2d::Vec2 key_viewer_end_position;
		};
	}
}
