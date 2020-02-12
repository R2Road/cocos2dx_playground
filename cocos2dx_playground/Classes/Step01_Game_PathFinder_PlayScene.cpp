#include "Step01_Game_PathFinder_PlayScene.h"

#include <new>
#include <sstream>
#include <random>

#include "Step01_Game_PathFinder_TitleScene.h"
#include "Step01_Game_Terrain_Viewer.h"

USING_NS_CC;

const int TAG_Player = 20140416;

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			PlayScene::PlayScene() :
				mKeyboardListener( nullptr )
				, mStageDataContainer()
				, mTerrainData()
				, mTerrainViewer( nullptr )
				, mCurrentStageIndex( 0u )
				, mPlayerPoint()
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
					ret->autorelease();
				}

				return ret;
			}

			bool PlayScene::init()
			{
				if( !Scene::init() )
					return false;

				const auto visibleSize = Director::getInstance()->getVisibleSize();
				const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

				//
				// Summury
				//
				{
					std::stringstream ss;
					ss << "+ " << getTitle();
					ss << "\n";
					ss << "\n";
					ss << "[ESC] : Return to Title";

					auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B::GREEN );
					label->setAnchorPoint( Vec2( 0.f, 1.f ) );
					label->setPosition( Vec2(
						visibleOrigin.x
						, visibleOrigin.y + visibleSize.height
					) );
					addChild( label, 9999 );
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

				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
				getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
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
				mPlayerPoint = mTerrainData.getPoint( step01::game::terrain::eTileType::entrance );
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
				step01::game::terrain::eTileType tile_type;
				for( int ty = 0; ty < mTerrainData.getHeight(); ++ty )
				{
					for( int tx = 0; tx < mTerrainData.getWidth(); ++tx )
					{
						tile_type = mTerrainData.get( tx, ty );
						if( step01::game::terrain::eTileType::exit == tile_type )
						{
							mTerrainViewer->UpdateTile( tx, ty, step01::game::terrain::eTileType::road );
						}
						else
						{
							mTerrainViewer->UpdateTile( tx, ty, mTerrainData.get( tx, ty ) );
						}
					}
				}
			}


			void PlayScene::MovePlayer( const int move_x, const int move_y )
			{
				mPlayerPoint.x += move_x;
				mPlayerPoint.y += move_y;

				auto player_node = mTerrainViewer->getChildByTag( TAG_Player );
				player_node->setPosition( mTerrainViewer->ConvertPoint2Position( mPlayerPoint.x, mPlayerPoint.y ) );

				const auto tile_type = mTerrainData.get( mPlayerPoint.x, mPlayerPoint.y );
				if( step01::game::terrain::eTileType::magic_circle_on == tile_type )
				{
					mTerrainData.set( mPlayerPoint.x, mPlayerPoint.y, step01::game::terrain::eTileType::magic_circle_off );
					mTerrainViewer->UpdateTile( mPlayerPoint.x, mPlayerPoint.y, step01::game::terrain::eTileType::magic_circle_off );

					const auto exit_point = mTerrainData.getPoint( step01::game::terrain::eTileType::exit );
					mTerrainViewer->UpdateTile( exit_point.x, exit_point.y, step01::game::terrain::eTileType::exit );
				}
			}


			void PlayScene::updateForExit( float /*dt*/ )
			{
				Director::getInstance()->replaceScene( game::pathfinder::TitleScene::create() );
			}

			void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
			{
				switch( keycode )
				{
				case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
					MovePlayer( -1, 0 );
					break;
				case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
					MovePlayer( 1, 0 );
					break;
				case EventKeyboard::KeyCode::KEY_UP_ARROW:
					MovePlayer( 0, 1 );
					break;
				case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
					MovePlayer( 0, -1 );
					break;

				case EventKeyboard::KeyCode::KEY_ESCAPE:
					if( !isScheduled( schedule_selector( PlayScene::updateForExit ) ) )
						scheduleOnce( schedule_selector( PlayScene::updateForExit ), 0.f );
					break;

				default:
					CCLOG( "Key Code : %d", keycode );
				}
			}
		} // namespace pathfinder
	}
}
