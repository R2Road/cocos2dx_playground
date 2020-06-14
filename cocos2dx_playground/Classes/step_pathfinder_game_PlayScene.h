#pragma once

#include "2d/CCScene.h"

#include "step_pathfinder_game_StageDataContainer.h"
#include "step_pathfinder_game_TerrainData.h"

namespace step_pathfinder
{
	namespace game
	{
		class TerrainViewer;

		class PlayScene : public cocos2d::Scene
		{
		private:
			enum class eNextSceneType
			{
				Title,
				Result,
			};

			PlayScene();

		public:
			static const char* getTitle() { return "Game : Play Scene"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void update( float dt ) override;
			void onExit() override;

		private:
			bool loadStage( std::size_t stage_index );
			bool goNextStage();
			void updateTerrainViewer();

			void GameProcess( const int move_x, const int move_y );

			void updateForExit( float dt );
			void startExitProcess( const eNextSceneType next_scene_type, float wait_time );

			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			int mAudioID_forBGM;

			StageDataContainer mStageDataContainer;
			TerrainData mTerrainData;
			TerrainViewer* mTerrainViewer;
			StageDataContainer::Container::size_type mCurrentStageIndex;
			TerrainPoint mPlayerPoint;
			bool mbPlayerLive;
			float mElapsedTime;
			eNextSceneType mNextSceneType;
		};
	}
}
