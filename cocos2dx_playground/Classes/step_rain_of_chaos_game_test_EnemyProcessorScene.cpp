#include "step_rain_of_chaos_game_test_EnemyProcessorScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_game_EnemyNode.h"
#include "step_rain_of_chaos_game_PlayerNode.h"
#include "step_rain_of_chaos_game_StageNode.h"

#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Fire.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Sleep.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Tie.h"
#include "step_rain_of_chaos_game_SpawnProcessor_MultipleShot_02_Line.h"
#include "step_rain_of_chaos_game_SpawnProcessor_Sleep.h"

USING_NS_CC;

namespace
{
	const int BulletCachingAmount = 100;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		EnemyProcessorScene::EnemyProcessorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )

			, mStartButton_MoveOffset()
			, mPlayerLastTouchPosition()

			, mStageConfig()
			, mStageNode( nullptr )

			, mStartNode( nullptr )
			, mTargetNode( nullptr )
		{}

		Scene* EnemyProcessorScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) EnemyProcessorScene( back_to_the_previous_scene_callback );
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

		bool EnemyProcessorScene::init()
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
				ss << "[SPACE] : Start Process";

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

			mStageConfig.Build(
				visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f
				, 120.f
			);

			//
			// Stage Node
			//
			{
				mStageNode = game::StageNode::create(
					mStageConfig
					, game::StageNode::DebugConfig{ false, true, true, true }
					, step_mole::CircleCollisionComponentConfig { false, false, false }
					, BulletCachingAmount
				);
				addChild( mStageNode );
			}

			//
			// Player Node
			//
			{
				auto player_node = game::PlayerNode::create( 1.f, game::PlayerNode::DebugConfig{ true }, step_mole::CircleCollisionComponentConfig{ true, true, true } );
				player_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				mStageNode->AddPlayer( player_node );

				// Move Helper
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->addTouchEventListener( CC_CALLBACK_2( EnemyProcessorScene::onTargetButton, this ) );
					player_node->addChild( button, std::numeric_limits<int>::max() - 1 );
				}

				mTargetNode = player_node;
			}

			//
			// Enemy Node
			//
			{
				Vec2 enemy_position = mStageConfig.GetCenter();
				enemy_position.y += ( mStageConfig.GetBulletGenerateRadiusMax() );

				auto enemy_node = game::EnemyNode::create(
					3.f
					, game::EnemyNode::DebugConfig{ true }
					, step_mole::CircleCollisionComponentConfig{ true, true, true }
					, std::bind( &game::StageNode::RequestBulletAction, mStageNode, std::placeholders::_1, std::placeholders::_2 )
				);
				enemy_node->setPosition( enemy_position );
				mStageNode->AddEnemy( enemy_node );

				// Move Helper
				{
					auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
					button->addTouchEventListener( CC_CALLBACK_2( EnemyProcessorScene::onStartButton, this ) );
					enemy_node->addChild( button, std::numeric_limits<int>::max() - 1 );
				}

				mStartNode = enemy_node;
			}

			//
			// Processor
			//
			{
				auto enemy_node = static_cast<game::EnemyNode*>( mStartNode );

				game::EnemyNode::EnemyProcessorContainer enemy_processor_container;
				enemy_processor_container.reserve( 100 );

				//enemy_processor_container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 180.f ) );
				//enemy_processor_container.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 180.f ) );

				//{
				//	game::SpawnProcessorContainer spawn_processor_container;
				//	spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 98.f, 8, 4, 0.1f ) );
				//	spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//	spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 52.f, 4, 3, 0.1f ) );
				//	spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
				//	spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 1, 4, 0.1f ) );
				//	spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );

				//	enemy_processor_container.emplace_back( game::EnemyProcessor_Fire::Create(
				//		mStageConfig
				//		, mStartNode
				//		, mTargetNode
				//		, std::move( spawn_processor_container )
				//		, enemy_node->GetSpawnInfoContainer()
				//	) );
				//}

				{
					game::SpawnProcessorContainer spawn_processor_container;
					spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 98.f, 8, 4, 0.1f ) );
					spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
					spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 52.f, 3, 3, 0.1f ) );
					spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );
					spawn_processor_container.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 2, 4, 0.1f ) );
					spawn_processor_container.emplace_back( game::SpawnProcessor_Sleep::Create( 0.3f ) );

					auto fire_processor = game::EnemyProcessor_Fire::Create(
						mStageConfig
						, mStartNode
						, mTargetNode
						, std::move( spawn_processor_container )
						, enemy_node->GetSpawnInfoContainer()
					);

					enemy_processor_container.emplace_back( game::EnemyProcessor_Tie::Create(
						mStageConfig
						, mStartNode
						, mTargetNode
						, std::move( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, mStartNode, mTargetNode, 2.f, true, 180.f ) )
						, std::move( fire_processor )
					) );
				}

				enemy_processor_container.emplace_back( game::EnemyProcessor_Sleep::Create( 1.f ) );
				enemy_processor_container.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, mStartNode, mTargetNode, 0.5f, true, 180.f ) );

				enemy_node->SetProcessor( std::move( enemy_processor_container ) );
			}

			return true;
		}

		void EnemyProcessorScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( EnemyProcessorScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void EnemyProcessorScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void EnemyProcessorScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_SPACE:
			{
				static_cast<game::EnemyNode*>( mStartNode )->StartProcess();
			}
			return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void EnemyProcessorScene::onStartButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mStartButton_MoveOffset = button->getParent()->getPosition() - button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				button->getParent()->setPosition( button->getTouchMovePosition() + mStartButton_MoveOffset );
			}
		}
		void EnemyProcessorScene::onTargetButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mPlayerLastTouchPosition = button->getTouchBeganPosition();
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				auto button = static_cast<ui::Button*>( sender );

				mStageNode->PlayerMoveRequest( button->getTouchMovePosition() - mPlayerLastTouchPosition );

				mPlayerLastTouchPosition = button->getTouchMovePosition();
			}
		}
	}
}
