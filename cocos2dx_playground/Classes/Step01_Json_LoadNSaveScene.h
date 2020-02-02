#pragma once

#include <string>
#include <vector>

#include "cocos2d.h"

namespace step01
{
	namespace json
	{
		class LoadNSaveScene : public cocos2d::Scene
		{
		private:
			LoadNSaveScene();

		public:
			~LoadNSaveScene();

			static const char* getTitle() { return "Json : Load And Save"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void SaveJsonFile();
			bool LoadJsonFile();

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			std::string mJsonString;
			std::vector<int> mDatas;
		};
	}
}
