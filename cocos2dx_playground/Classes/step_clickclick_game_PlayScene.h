#pragma once

#include <memory>

#include "2d/CCScene.h"

#include "cpg_GridIndexConverter.h"

namespace step_clickclick
{
	namespace game
	{
		using StageUp = std::unique_ptr<class Stage>;
		class StageView;

		class PlayScene : public cocos2d::Scene
		{
		private:
			PlayScene();

		public:
			static const char* getTitle() { return "Game : Play"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onGameProcess( const int pannel_linear_index );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			StageUp mStage;
			StageView* mStageView;
			const cpg::GridIndexConverter mGridIndexConverter;
		};
	}
}
