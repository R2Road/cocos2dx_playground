#pragma once

#include <memory>
#include <vector>

#include "2d/CCScene.h"

#include "step_clickclick_game_Constant.h"
#include "step_clickclick_game_Block.h"


namespace step_clickclick
{
	namespace game
	{
		class BlockViewNode;
	}

	namespace game_test
	{
		class BlockScene : public cocos2d::Scene
		{
		private:
			enum class eTestActionType
			{
				Increase,
				Decrease,
				Die,
			};

			BlockScene();

		public:
			static const char* getTitle() { return "Game Test : Block"; }
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onGameProcess( const int block_linear_index );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

			void updateTestAction( const eTestActionType test_action_type );
			void updateSelectedBlockTypeView( const step_clickclick::game::eBlockType block_type );

			void ResetBlockContainer();

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			eTestActionType mTestActionType;

			std::vector<step_clickclick::game::Block> mBlockContainer;
			std::vector<step_clickclick::game::BlockViewNode*> mBlockViewContainer;
		};
	}
}
