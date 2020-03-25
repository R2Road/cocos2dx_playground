#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "cpg_input_KeyCodeCollector.h"

namespace step_rain_of_chaos
{
	namespace collision
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Collision : Basic"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void update( float dt ) override;
			void onExit() override;

		private:
			void updateDistance();

			void updateMoveSpeedView();

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			
			cpg::input::KeyCodeCollector mKeyCodeCollector;
			int mMoveSpeed;
		};
	}
}
