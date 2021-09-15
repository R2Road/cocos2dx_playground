#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
	class DrawNode;
	class Label;
NS_CC_END

namespace ui_research
{
	namespace gauge_research
	{
		class BasicScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			BasicScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Basic"; }
			static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void updateGaugeStatisticsView();
			void updateGaugeView();

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* key_event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cocos2d::DrawNode* mGaugeViewNode;
			const int mGaugeMax;
			const int mGaugeMin;
			int mGaugeCurrent;

			cocos2d::Label* mGaugeStatisticsViewNode;
		};
	}
}
