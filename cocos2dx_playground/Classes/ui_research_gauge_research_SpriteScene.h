#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

NS_CC_BEGIN
	class Label;
	class Sprite;
NS_CC_END

namespace ui_research
{
	namespace gauge_research
	{
		class SpriteScene : public cocos2d::Scene, private helper::BackToThePreviousScene
		{
		private:
			SpriteScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		public:
			static const char* getTitle() { return "Sprite"; }
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

			cocos2d::Node* mGaugeViewNode1;
			cocos2d::Node* mGaugeViewNode2;
			cocos2d::Sprite* mGaugeViewNode3;
			cocos2d::Rect mGaugeViewNode3TextureRect;

			const int mGaugeMax;
			const int mGaugeMin;
			int mGaugeCurrent;

			cocos2d::Label* mGaugeStatisticsViewNode;
		};
	}
}
