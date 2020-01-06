#pragma once

#include <list>

#include "cocos2d.h"
#include "ui/UIWidget.h"

namespace cpg
{
	namespace input
	{
		using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;
	}

	namespace input_test
	{
		class KeyViewer;
	}
}

namespace research
{
	namespace input
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
			cpg::input::KeyCollectorSp input_collector;
			std::list<cpg::input_test::KeyViewer*> key_viewer_list;
			cocos2d::Vec2 key_viewer_start_position;
			cocos2d::Vec2 key_viewer_end_position;
		};
	}
}
