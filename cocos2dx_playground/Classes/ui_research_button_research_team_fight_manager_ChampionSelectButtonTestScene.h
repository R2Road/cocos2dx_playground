#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

namespace ui_research
{
	namespace button_research
	{
		namespace team_fight_manager
		{
			class ChampionSelectButtonTestScene : public cocos2d::Scene, private helper::BackToThePreviousScene
			{
			private:
				ChampionSelectButtonTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			public:
				static const char* getTitle() { return "TFM Champion Select Button"; }
				static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

			private:
				bool init() override;

			public:
				void onEnter() override;
				void onExit() override;

			private:
				void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

			private:
				cocos2d::EventListenerKeyboard* mKeyboardListener;
			};
		}
	}
}
