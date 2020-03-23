#include "step_pathfinder_game_PlayScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_pathfinder_game_TitleScene.h"
#include "step_pathfinder_game_ResultScene.h"
#include "step_pathfinder_game_terrain_Viewer.h"

USING_NS_CC;

namespace
{
	const int TAG_Player = 20140416;
	const int TAG_GameOver = 20200209;
}

namespace step_pathfinder
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mStageDataContainer()
			, mTerrainData()
			, mTerrainViewer( nullptr )
			, mCurrentStageIndex( 0u )
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
				return nullptr;
			}
			else
			{
				ret->scheduleUpdate();
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

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "[ESC] : Return to Title";
				ss << std::endl;
				ss << "[ARROW] : Move";


				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
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
					auto label = Label::createWithTTF( "Game Over", "fonts/arial.ttf", 20 );
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
				mStageDataContainer.load();
			}

			//
			// Terrain View
			//
			{
				mTerrainViewer = terrain::Viewer::create( mTerrainData.getWidth(), mTerrainData.getHeight() );
				mTerrainViewer->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mTerrainViewer->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mTerrainViewer->getContentSize().height ) * 0.5f )
				) );
				addChild( mTerrainViewer );

				updateTerrainViewer();
			}

			//
			// Player
			//
			{
				auto player_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
				player_node->setTag( TAG_Player );
				mTerrainViewer->addChild( player_node, 100 );
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.2f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );

					player_node->runAction( repeat_action );
				}
			}

			//
			// Load Stage
			//
			{
				assert( mStageDataContainer.size() > mCurrentStageIndex );
				loadStage( mCurrentStageIndex );
			}

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void PlayScene::update( float dt )
		{
			mElapsedTime += dt;

			Scene::update( dt );
		}
		void PlayScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}


		bool PlayScene::loadStage( std::size_t stage_index )
		{
			if( !mTerrainData.load( mStageDataContainer.get( stage_index ).c_str() ) )
			{
				return false;
			}

			updateTerrainViewer();

			auto player_node = mTerrainViewer->getChildByTag( TAG_Player );
			mPlayerPoint = mTerrainData.getPoint( step_pathfinder::game::terrain::eTileType::entrance );
			player_node->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );

			return true;
		}
		bool PlayScene::goNextStage()
		{
			if( !loadStage( mCurrentStageIndex + 1 ) )
			{
				return false;
			}

			++mCurrentStageIndex;
			return true;
		}
		void PlayScene::updateTerrainViewer()
		{
			// apply terrain data
			step_pathfinder::game::terrain::eTileType tile_type;
			for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
			{
				for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
				{
					tile_type = mTerrainData.get( tx, ty );
					if( step_pathfinder::game::terrain::eTileType::exit == tile_type )
					{
						mTerrainViewer->UpdateTile( tx, ty, step_pathfinder::game::terrain::eTileType::road );
					}
					else
					{
						mTerrainViewer->UpdateTile( tx, ty, mTerrainData.get( tx, ty ) );
					}
				}
			}
		}


		void PlayScene::GameProcess( const int move_x, const int move_y )
		{
			//
			// sfx
			//
			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.2f );

			//
			// move player
			//
			mPlayerPoint.x += move_x;
			mPlayerPoint.y += move_y;

			//
			// move player view
			//
			auto player_node = mTerrainViewer->getChildByTag( TAG_Player );
			player_node->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );

			//
			// check player position
			//
			const auto tile_type = mTerrainData.get( mPlayerPoint.x, mPlayerPoint.y );
			if( step_pathfinder::game::terrain::eTileType::damage == tile_type )
			{
				// player die
				mbPlayerLive = false;
				getChildByTag( TAG_GameOver )->setVisible( true );

				// move to title
				startExitProcess( eNextSceneType::Title, 3.f );
			}
			else if( step_pathfinder::game::terrain::eTileType::magic_circle_on == tile_type )
			{
				// convert tile data : magic_circle_on > magic_circle_off
				mTerrainData.set( mPlayerPoint.x, mPlayerPoint.y, step_pathfinder::game::terrain::eTileType::magic_circle_off );
				mTerrainViewer->UpdateTile( mPlayerPoint.x, mPlayerPoint.y, step_pathfinder::game::terrain::eTileType::magic_circle_off );

				// update tile view
				const auto exit_point = mTerrainData.getPoint( step_pathfinder::game::terrain::eTileType::exit );
				mTerrainViewer->UpdateTile( exit_point.x, exit_point.y, step_pathfinder::game::terrain::eTileType::exit );
			}
			else if( step_pathfinder::game::terrain::eTileType::exit == tile_type )
			{
				if( !mTerrainData.isExist( step_pathfinder::game::terrain::eTileType::magic_circle_on ) )
				{
					if( mStageDataContainer.size() == mCurrentStageIndex + 1 )
					{
						// game clear
						mbPlayerLive = false;
						startExitProcess( eNextSceneType::Result, 1.f );
					}
					else
					{
						// go next stage
						goNextStage();
					}
				}
			}
		}


		void PlayScene::updateForExit( float /*dt*/ )
		{
			if( eNextSceneType::Title == mNextSceneType )
			{
				Director::getInstance()->replaceScene( game::TitleScene::create() );
			}
			else
			{
				Director::getInstance()->replaceScene( game::ResultScene::create( mElapsedTime ) );
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
