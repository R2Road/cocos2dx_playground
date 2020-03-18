#pragma once

#include <string>
#include <vector>

#include "2d/CCScene.h"

namespace step01
{
	namespace json
	{
		class LoadNSaveScene : public cocos2d::Scene
		{
		private:
			LoadNSaveScene();

		public:
			static const char* getTitle() { return "Json : Load And Save"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void makeDummyJsonFile( const char* json_path );
			void saveJsonFile( const char* json_path );
			bool loadJsonFile( const char* json_path );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			std::string mJsonString;
			std::vector<int> mDatas;
		};
	}
}
