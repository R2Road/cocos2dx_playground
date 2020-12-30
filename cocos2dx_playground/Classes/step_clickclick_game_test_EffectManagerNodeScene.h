#pragma once

#include "2d/CCScene.h"

#include "cpg_GridIndexConverter.h"
#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		class EffectManagerNode;
	}

	namespace game_test
	{
		class EffectManagerNodeScene : public cocos2d::Scene
		{
		private:
			EffectManagerNodeScene();

		public:
			static const char* getTitle() { return "Game Test : Effect Manager"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void PlayAllEffect( const game::eEffectIndex effect_index );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			game::EffectManagerNode* mEffectManagerNode;
			cpg::GridIndexConverter mGridIndexConverter;
		};
	}
}
