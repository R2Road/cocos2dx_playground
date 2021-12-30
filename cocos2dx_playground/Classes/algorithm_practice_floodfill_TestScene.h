#pragma once

#include "2d/CCScene.h"

#include "algorithm_practice_floodfill_Grid4TileMap.h"
#include "helper_BackToThePreviousScene.h"

namespace step_defender
{
	namespace game
	{
		class TileMapNode;
	}
}

namespace algorithm_practice_floodfill
{
	class EditorNode;
	class ProcessorNode;

	class TestScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		enum eMode
		{
			Edit,
			Process,
		};

		TestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Test"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onModeSelect( const int mode_index );	

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode key_code, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		eMode mMode;

		Grid4TileMap mGrid4TileMap;

		step_defender::game::TileMapNode* mTileMapNode;
		cocos2d::Node* mEntryPointIndicatorNode;
		EditorNode* mEditorNode;
		ProcessorNode* mProcessorNode;
	};
}
