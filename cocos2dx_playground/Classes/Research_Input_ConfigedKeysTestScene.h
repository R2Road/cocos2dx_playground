#pragma once

#include <list>

#include "cocos2d.h"
#include "ui/UIWidget.h"

namespace CPG
{
	namespace Input
	{
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
		class ConfigedKeysTestScene : public cocos2d::Scene
		{
		public:
			ConfigedKeysTestScene();

			static cocos2d::Scene* create();

			bool init() override;
			void update( float dt ) override;

		private:
			void onExitButton( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );
			void update_forExit( float dt );

		private:
			CPG::Input::KeyCollectorSp input_collector;
			std::list<CPG::InputTest::KeyViewer*> key_viewer_list;
			cocos2d::Vec2 key_viewer_start_position;
			cocos2d::Vec2 key_viewer_end_position;
		};
	}
}
