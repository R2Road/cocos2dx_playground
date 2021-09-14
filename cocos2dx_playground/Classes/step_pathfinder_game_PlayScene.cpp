#include "step_pathfinder_game_PlayScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_pathfinder_game_PlayerNode.h"
#include "step_pathfinder_game_ResultScene.h"
#include "step_pathfinder_game_TerrainViewer.h"
#include "step_pathfinder_game_TitleScene.h"

USING_NS_CC;

namespace
{
	const int TAG_GameOver = 20200209;
}

namespace step_pathfinder
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mAudioID_forBGM( -1 )

			, mStageDataContainer()
			, mCurrentStageDataIndex( 0u )

			, mTerrainData()
			, mTerrainViewer( nullptr )
			, mPlayerNode( nullptr )

			, mPlayerPoint()
			, mbPlayerLive( true )
			, mElapsedTime( 0.f )
			, mNextSceneType( eNextSceneType::Title )
		{}

		Scene* PlayScene::create()
		{
			auto ret = new ( std::nothrow ) PlayScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool PlayScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "[ESC] : Return to Title";
				ss << cpg::linefeed;
				ss << "[ARROW] : Move";


				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// BGM License
			//
			{
				auto label = Label::createWithTTF(
					"BGM : Empty Space\nAuthor : tcarisland\nLicense : CC-BY 4.0\nFrom : https://opengameart.org/"
					, cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::RIGHT
				);
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Game Over
			//
			{
				auto game_over_indicator = LayerColor::create( Color4B( 50, 50, 50, 180 ), visibleSize.width, visibleSize.height * 0.3f );
				game_over_indicator->setTag( TAG_GameOver );
				game_over_indicator->setVisible( false );
				game_over_indicator->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + ( ( visibleSize.height - game_over_indicator->getContentSize().height ) * 0.5f )
				)
				);
				addChild( game_over_indicator, std::numeric_limits<int>::max() - 1 );
				{
					auto label = Label::createWithTTF( "Game Over", cpg::StringTable::GetFontPath(), 20 );
					label->setColor( Color3B::RED );
					label->setPosition( Vec2(
						game_over_indicator->getContentSize().width * 0.5f
						, game_over_indicator->getContentSize().height * 0.5f
					) );
					game_over_indicator->addChild( label );
				}
			}

			//
			// Stage Datas
			//
			{
				mStageDataContainer.Load();
			}

			//
			// Terrain View
			//
			{
				mTerrainViewer = TerrainViewer::create( mTerrainData.GetWidth(), mTerrainData.GetHeight(), Size( 48, 48 ) );
				mTerrainViewer->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mTerrainViewer->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mTerrainViewer->getContentSize().height ) * 0.5f )
				) );
				addChild( mTerrainViewer );
			}

			//
			// Player
			//
			{
				mPlayerNode = PlayerNode::create();
				mTerrainViewer->addChild( mPlayerNode, 100 );
			}

			//
			// Load Stage
			//
			{
				assert( mStageDataContainer.Size() > mCurrentStageDataIndex );
				loadStage( mCurrentStageDataIndex );
			}

			schedule( schedule_selector( PlayScene::update4PlayTime ) );

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			mAudioID_forBGM = experimental::AudioEngine::play2d( "sounds/bgm/EmpySpace.ogg", true, 0.1f );

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayScene::onExit()
		{
			experimental::AudioEngine::stop( mAudioID_forBGM );
			mAudioID_forBGM = -1;

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void PlayScene::update4PlayTime( float dt )
		{
			mElapsedTime += dt;
		}


		bool PlayScene::loadStage( std::size_t stage_index )
		{
			if( !mTerrainData.Load( mStageDataContainer.Get( stage_index ).c_str() ) )
			{
				return false;
			}

			mTerrainViewer->LoadTerrainData4Game( mTerrainData );

			mPlayerPoint = mTerrainData.FindTilePoint( eTileType::entrance );
			mPlayerNode->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );

			return true;
		}
		bool PlayScene::goNextStage()
		{
			const auto target_stage_data_index = mCurrentStageDataIndex + 1;
			if( !loadStage( target_stage_data_index ) )
			{
				return false;
			}

			mCurrentStageDataIndex = target_stage_data_index;

			return true;
		}


		void PlayScene::GameProcess( const int move_x, const int move_y )
		{
			//
			// move player
			//
			mPlayerPoint.x += move_x;
			mPlayerPoint.y += move_y;
			mPlayerNode->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );

			//
			// check player position
			//
			const auto tile_type = mTerrainData.Get( mPlayerPoint.x, mPlayerPoint.y );
			if( eTileType::damage == tile_type )
			{
				experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );

				// player die
				mbPlayerLive = false;
				getChildByTag( TAG_GameOver )->setVisible( true );

				// move to title
				startExitProcess( eNextSceneType::Title, 3.f );
			}
			else if( eTileType::magic_circle_on == tile_type )
			{
				experimental::AudioEngine::play2d( "sounds/fx/coin_001.ogg", false, 0.2f );

				// convert tile data : magic_circle_on > magic_circle_off
				mTerrainData.Set( mPlayerPoint.x, mPlayerPoint.y, eTileType::magic_circle_off );
				mTerrainViewer->UpdateTile( mPlayerPoint.x, mPlayerPoint.y, eTileType::magic_circle_off );

				// update tile view
				const auto exit_point = mTerrainData.FindTilePoint( eTileType::exit );
				mTerrainViewer->UpdateTile( exit_point.x, exit_point.y, eTileType::exit );
			}
			else if( eTileType::exit == tile_type && !mTerrainData.isExist( eTileType::magic_circle_on ) )
			{
				experimental::AudioEngine::play2d( "sounds/fx/powerup_001.ogg", false, 0.1f );

				if( !goNextStage() )
				{
					// game clear
					mbPlayerLive = false;
					startExitProcess( eNextSceneType::Result, 1.f );
				}
			}
			else
			{
				experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );
			}
		}


		void PlayScene::updateForExit( float /*dt*/ )
		{
			if( eNextSceneType::Title == mNextSceneType )
			{
				_director->replaceScene( game::TitleScene::create() );
			}
			else
			{
				_director->replaceScene( game::ResultScene::create( mElapsedTime ) );
			}
		}
		void PlayScene::startExitProcess( const eNextSceneType next_scene_type, float wait_time )
		{
			if( !isScheduled( schedule_selector( PlayScene::updateForExit ) ) )
			{
				mNextSceneType = next_scene_type;
				scheduleOnce( schedule_selector( PlayScene::updateForExit ), wait_time );
			}
		}

		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( mbPlayerLive )
			{
				switch( keycode )
				{
				case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
					GameProcess( -1, 0 );
					break;
				case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
					GameProcess( 1, 0 );
					break;
				case EventKeyboard::KeyCode::KEY_UP_ARROW:
					GameProcess( 0, 1 );
					break;
				case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
					GameProcess( 0, -1 );
					break;
				}
			}

			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				startExitProcess( eNextSceneType::Title, 0.f );
			}
		}
	}
}
