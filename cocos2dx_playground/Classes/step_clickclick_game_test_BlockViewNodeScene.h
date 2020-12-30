#pragma once

#include "2d/CCScene.h"

#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		class BlockViewNode;
	}

	namespace game_test
	{
		class BlockViewNodeScene : public cocos2d::Scene
		{
		private:
			enum class eTestActionType
			{
				Increase,
				Decrease,
				Die,
			};

			BlockViewNodeScene();

		public:
			static const char* getTitle() { return "Game Test : Block View Node"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onBlock( const int block_index );
			void updateTestAction( const eTestActionType test_action_type );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			eTestActionType mTestActionType;

			step_clickclick::game::eBlockType mBlockType;
			int mBlockLife;
			step_clickclick::game::BlockViewNode* mBlockViewNode;
		};
	}
}
