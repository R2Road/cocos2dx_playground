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
		class StageView;

		class BlockTestScene : public cocos2d::Scene
		{
		private:
			enum class eTestActionType
			{
				Increase,
				Decrease,
				Die,
			};

			BlockTestScene();

		public:
			static const char* getTitle() { return "Game : Block Test"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onGameProcess( const int block_linear_index );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			void updateTestAction( const eTestActionType test_action_type );
			void updateSelectedBlockTypeView( const eBlockType block_type );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			eTestActionType mTestActionType;

			std::vector<Block> mBlockContainer;
		};
	}
}
