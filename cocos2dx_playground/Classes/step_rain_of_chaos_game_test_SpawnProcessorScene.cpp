#include "step_rain_of_chaos_game_test_SpawnProcessorScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_game_StageNode.h"
#include "step_rain_of_chaos_game_BulletManager.h"

USING_NS_CC;

namespace
{
	const int BulletCachingAmount = 100;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		SpawnProcessorScene::SpawnProcessorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mStageConfig()
			, mBulletManager( nullptr )
			, mStageNode( nullptr )
			, mSpawnProcessorContainer()
		{}

		Scene* SpawnProcessorScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) SpawnProcessorScene( back_to_the_previous_scene_callback );
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

		bool SpawnProcessorScene::init()
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
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B::BLACK );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Target Manager
			//
			{
				mBulletManager = game::BulletManager::create( BulletCachingAmount );
			}

			//
			// Stage Node
			//
			{
				mStageConfig.Build(
					visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f
					, 300.f, 160.f
				);

				mStageNode = game::StageNode::create(
					mStageConfig
					, game::StageNode::DebugConfig{ true, true }
					, mBulletManager->GetComeHomeCallback()
					, step_mole::CircleCollisionComponentConfig { true, true, true }
					, BulletCachingAmount
				);
				addChild( mStageNode );
			}

			//
			// Spawn Processor
			//
			{
				mSpawnProcessorContainer.emplace_back( game::TestSpawnProcessor::Create() );
				mSpawnProcessorContainer.emplace_back( game::TestSpawnProcessor::Create() );
				mSpawnProcessorContainer.emplace_back( game::TestSpawnProcessor::Create() );
				mCurrentSpawnProcessor = mSpawnProcessorContainer.begin();
			}

			schedule( schedule_selector( SpawnProcessorScene::updateForSpawnProcessor ) );

			return true;
		}

		void SpawnProcessorScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpawnProcessorScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void SpawnProcessorScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void SpawnProcessorScene::updateForSpawnProcessor( float dt )
		{
			if( mSpawnProcessorContainer.end() == mCurrentSpawnProcessor )
			{
				unschedule( schedule_selector( SpawnProcessorScene::updateForSpawnProcessor ) );
				return;
			}

			game::SpawnInfoContainer aaa;

			if( !( *mCurrentSpawnProcessor )->Update( dt, &aaa ) )
			{
				++mCurrentSpawnProcessor;
			}

			if( !aaa.empty() )
			{
				int target_index = -1;
				for( const auto& s : aaa )
				{
					target_index = mBulletManager->GetIdleTarget();
					if( -1 == target_index )
					{
						mBulletManager->RequestGenerate( 50 );
						mStageNode->RequestGenerate( 50 );

						target_index = mBulletManager->GetIdleTarget();
						if( -1 == target_index )
						{
							break;
						}
					}

					Vec2 dir = s.MoveDirection;
					dir.normalize();
					dir.scale( 3.f );
					mStageNode->RequestAction( target_index, s.StartPosition, dir );
				}
			}
		}

		void SpawnProcessorScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
