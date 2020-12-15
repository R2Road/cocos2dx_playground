#pragma once

#include <list>

#include "2d/CCScene.h"
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

namespace input_practice
{
	class ConfigedKeysTestScene : public cocos2d::Scene
	{
	private:
		ConfigedKeysTestScene();

	public:
		static const char* getTitle() { return "Configed Keys Test"; }
		static cocos2d::Scene* create();

	private:
		bool init() override;

	public:
		void update( float dt ) override;

	private:
		void onExitButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void update_forExit( float dt );

	private:
		cpg::input::KeyCollectorSp mInputCollector;
		std::list<cpg::input_test::KeyViewer*> mKeyViewerList;
		cocos2d::Vec2 mKeyViewer_StartPosition;
		cocos2d::Vec2 mKeyViewer_EndPosition;
	};
}