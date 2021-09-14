#include "step_mole_game_test_SpawnAndHitScene.h"

#include <algorithm>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_mole_game_StageNode.h"
#include "step_mole_game_TargetManager.h"
#include "step_mole_game_HittingNode.h"

USING_NS_CC;

namespace
{
	const step_mole::game::StageConfig STAGE_CONFIG{ 5, 4, Size( 40.f, 40.f ) };

	const int TAG_GroupSpawnCountNode = 20140416;
}

namespace step_mole
{
	namespace game_test
	{
		SpawnAndHitScene::SpawnAndHitScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTargetManager()
			, mStageNode( nullptr )

			, mCurrentSpawnTargetCount( 3 )
		{}

		Scene* SpawnAndHitScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) SpawnAndHitScene( back_to_the_previous_scene_callback );
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

		bool SpawnAndHitScene::init()
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
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow Up] : Increase Spawn Count";
				ss << cpg::linefeed;
				ss << "[Arrow Down] : Decrease Spawn Count";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Spawn Target Count
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_GroupSpawnCountNode );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateSpawnTargetCountView();
			}

			//
			// Target Manager
			//
			{
				mTargetManager = game::TargetManager::create( STAGE_CONFIG );
			}

			//
			// Stage Node
			//
			{
				mStageNode = step_mole::game::StageNode::create(
					STAGE_CONFIG
					, std::bind( &game::TargetManager::ComeHomeTarget, mTargetManager.get(), std::placeholders::_1 )
					, game::StageNode::DebugConfig{ true, true }
					, CircleCollisionComponentConfig{ true, true, true }
				);
				mStageNode->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mStageNode->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mStageNode->getContentSize().height ) * 0.5f )
				) );
				addChild( mStageNode );
			}

			//
			// Hitting Node
			//
			{
				auto hitting_node = step_mole::game::HittingNode::create(
					{ STAGE_CONFIG.BlockCount_Horizontal + 1, STAGE_CONFIG.BlockCount_Vercital + 1, STAGE_CONFIG.BlockSize }
					, game::HittingNodeConfig{ false, false }
					, std::bind( &SpawnAndHitScene::attackProcess, this, std::placeholders::_1, std::placeholders::_2 )
				);
				hitting_node->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - hitting_node->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - hitting_node->getContentSize().height ) * 0.5f )
				) );
				addChild( hitting_node );
			}

			//
			// Process Start
			//
			{
				scheduleOnce( SEL_SCHEDULE( &SpawnAndHitScene::updateForSpawnProcessStart ), 1.f );
			}

			return true;
		}

		void SpawnAndHitScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SpawnAndHitScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void SpawnAndHitScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void SpawnAndHitScene::updateForSpawnProcessStart( const float /*dt*/ )
		{
			CCLOG( "Start - SpawnAndHitScene::updateForSpawn" );
			schedule( SEL_SCHEDULE( &SpawnAndHitScene::updateForSpawn ), 2.f );
		}
		void SpawnAndHitScene::updateForSpawn( const float /*dt*/ )
		{
			int target_index = -1;
			for( int i = 0; i < mCurrentSpawnTargetCount; ++i )
			{
				target_index = mTargetManager->GetIdleTarget();
				if( -1 == target_index )
				{
					break;
				}

				mStageNode->RequestAction( target_index, 2.f );
			}
		}
		void SpawnAndHitScene::attackProcess( const int world_x, const int world_y )
		{
			if( 0 < mStageNode->RequestAttack( world_x, world_y ) )
			{
				CCLOG( "success" );
			}
			else // miss callback
			{
				CCLOG( "miss" );
			}
		}

		void SpawnAndHitScene::updateSpawnTargetCountView()
		{
			auto group_spawn_count_node = static_cast<Label*>( getChildByTag( TAG_GroupSpawnCountNode ) );
			group_spawn_count_node->setString( StringUtils::format( "Group Spawn Count : %d", mCurrentSpawnTargetCount ) );
		}

		void SpawnAndHitScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mCurrentSpawnTargetCount += 1;
				updateSpawnTargetCountView();
				return;

			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mCurrentSpawnTargetCount = std::max( 1, mCurrentSpawnTargetCount - 1 );
				updateSpawnTargetCountView();
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
