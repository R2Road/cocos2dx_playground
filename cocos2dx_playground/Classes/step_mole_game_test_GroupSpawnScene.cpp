#include "step_mole_game_test_GroupSpawnScene.h"

#include <algorithm>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_mole_game_StageNode.h"
#include "step_mole_game_TargetManager.h"
#include "step_mole_game_HittingNode.h"

USING_NS_CC;

namespace
{
	const step_mole::game::StageConfig STAGE_CONFIG{ 3, 3, Size( 40.f, 40.f ) };

	const int TAG_GroupSpawnCountNode = 20140416;
}

namespace step_mole
{
	namespace game_test
	{
		GroupSpawnScene::GroupSpawnScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mTargetManager()
			, mStageNode( nullptr )

			, mCurrentSpawnTargetCount( 3 )
		{}

		Scene* GroupSpawnScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) GroupSpawnScene( back_to_the_previous_scene_callback );
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

		bool GroupSpawnScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[A] : Group Spawn";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				addChild( background_layer, -1 );
			}

			//
			// Spawn Target Count
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
					, game::StageNodeConfig{ true, true }
					, CircleCollisionComponentConfig{ true, true, true }
				);
				mStageNode->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mStageNode->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mStageNode->getContentSize().height ) * 0.5f )
				) );
				addChild( mStageNode );
			}

			return true;
		}

		void GroupSpawnScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GroupSpawnScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void GroupSpawnScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void GroupSpawnScene::updateSpawnTargetCountView()
		{
			auto group_spawn_count_node = static_cast<Label*>( getChildByTag( TAG_GroupSpawnCountNode ) );
			group_spawn_count_node->setString( StringUtils::format( "Group Spawn Count : %d", mCurrentSpawnTargetCount ) );
		}

		void GroupSpawnScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_A:
			{
				int target_index = -1;
				for( int i = 0; i < mCurrentSpawnTargetCount; ++i )
				{
					target_index = mTargetManager->GetIdleTarget();
					if( -1 == target_index )
					{
						break;
					}

					mStageNode->RequestAction( target_index, 3.f );
				}
			}
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
