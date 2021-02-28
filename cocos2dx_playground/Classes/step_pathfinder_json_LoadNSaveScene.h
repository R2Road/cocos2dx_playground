#pragma once

#include <string>
#include <vector>

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace step_pathfinder
{
	namespace json
	{
		class LoadNSaveScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			using ContainerT = std::vector<int>;

			LoadNSaveScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Json : Load And Save"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void makeDummyJsonFile( const char* json_path ) const;
			void saveJsonFile( const char* json_path, const ContainerT& json_datas ) const;
			bool loadJsonFile( const char* json_path, std::string& out_json_string, ContainerT& out_json_datas );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
