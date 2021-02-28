#pragma once

#include <string>

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

namespace step_pathfinder
{
	namespace fileutil
	{
		class GenerateNDeleteScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			GenerateNDeleteScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback, std::string&& file_full_path );

		public:
			static const char* getTitle() { return "FileUtil : Generate N Delete"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateFileStatus();

			void onGenerateButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void onDeleteButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			const std::string mFileFullPath;
		};
	}
}
