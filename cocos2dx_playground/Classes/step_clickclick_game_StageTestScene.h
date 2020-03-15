#pragma once

#include <memory>

#include "2d/CCScene.h"

#include "cpg_GridIndexConverter.h"
#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		using StageUp = std::unique_ptr<class Stage>;
		class StageView;

		class StageTestScene : public cocos2d::Scene
		{
		private:
			enum class eTestActionType
			{
				Increase,
				Decrease,
				Die,
			};

			StageTestScene();

		public:
			static const char* getTitle() { return "Game : Stage Test"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onGameProcess( const int block_linear_index );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			void updateTestAction( const eTestActionType test_action_type );
			void updateSelectedBlockTypeView( const eBlockType block_type );
			void updateActiveBlockCountView( const int count );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			StageUp mStage;
			StageView* mStageView;
			const cpg::GridIndexConverter mGridIndexConverter;

			eTestActionType mTestActionType;
		};
	}
}
