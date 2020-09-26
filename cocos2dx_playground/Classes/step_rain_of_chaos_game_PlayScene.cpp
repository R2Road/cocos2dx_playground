#include "step_rain_of_chaos_game_PlayScene.h"

#include <cstdlib> // ldiv
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

#include "cpg_Random.h"

#include "step_rain_of_chaos_game_BackgroundNode.h"
#include "step_rain_of_chaos_game_PlayerNode.h"
#include "step_rain_of_chaos_game_StageNode.h"

#include "step_rain_of_chaos_game_EnemyProcessor_Blink_CircularSector_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Fire_Chain.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Fire_Single.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_Random_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_CircularSector_Random_02.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Linear_2Target_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Move_Orbit_01.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Sleep.h"
#include "step_rain_of_chaos_game_EnemyProcessor_Tie.h"

#include "step_rain_of_chaos_game_SpawnProcessor_MultipleShot_01_CircularSector.h"
#include "step_rain_of_chaos_game_SpawnProcessor_MultipleShot_02_Line.h"
#include "step_rain_of_chaos_game_SpawnProcessor_SingleShot_01.h"
#include "step_rain_of_chaos_game_SpawnProcessor_SingleShot_02_Spread.h"
#include "step_rain_of_chaos_game_SpawnProcessor_Sleep.h"

#include "step_rain_of_chaos_game_TitleScene.h"

USING_NS_CC;

namespace
{
	const int BulletCachingAmount = 200;

	const int TAG_FadeIn = 10001;
	const int TAG_Player = 10002;
	const int TAG_Enemy = 10003;
	const int TAG_CenterPivot = 10004;
	const int TAG_Ready = 10005;
	const int TAG_Go = 10006;

	const float ScrollScale = 0.15f;
}

namespace step_rain_of_chaos
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mKeyCodeCollector()

			, mStageConfig()
			, mStageNode( nullptr )
			, mBackgroundNode( nullptr )

			, mStep( eStep::Test )
			, mPackgeContainer()
			, mPackageIndicator( 0u )
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

			schedule( schedule_selector( PlayScene::update4Intro) );

			const auto visibleOrigin = _director->getVisibleOrigin();
			const auto visibleSize = _director->getVisibleSize();
			const auto visibleCenter = visibleOrigin + Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f );

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "[ESC] : Return to Title";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 8, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() - 1 );
			}

			//
			// BGM License
			//
			{
				auto label = Label::createWithTTF(
					"BGM : Empty Space\nAuthor : tcarisland\nLicense : CC-BY 4.0\nFrom : https://opengameart.org/"
					, "fonts/NanumSquareR.ttf", 8, Size::ZERO, TextHAlignment::RIGHT
				);
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() - 1 );
			}

			mStageConfig.Build(
				visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f
				, 120.f
			);

			//
			// Background Node
			//
			{
				const auto tile_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_0.png" )->getOriginalSizeInPixels();

				const auto div_result_width = std::div( static_cast<int>( visibleSize.width + mStageConfig.GetStageRect().size.width * ScrollScale ), static_cast<int>( tile_size.width ) );
				const std::size_t vertical_amount = div_result_width.rem > 0 ? div_result_width.quot + 1 : div_result_width.quot;

				const auto div_result_height = std::div( static_cast<int>( visibleSize.height + mStageConfig.GetStageRect().size.height * ScrollScale ), static_cast<int>( tile_size.height ) );
				const std::size_t horizontal_amount = div_result_height.rem > 0 ? div_result_height.quot + 1 : div_result_height.quot;

				std::vector<SpriteFrame*> SpriteFrames{
					SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_0.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_0.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_1.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_2.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_3.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_4.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_5.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_6.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_7.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_8.png" )
					, SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_9.png" )
				};

				auto background_node = step_rain_of_chaos::game::BackgroundNode::create( 10, 10, "textures/texture_001.png", std::move( SpriteFrames ) );
				background_node->Reset( vertical_amount, horizontal_amount );
				background_node->setPosition(
					-mStageConfig.GetStageRect().size.width * 0.5f * ScrollScale
					, -mStageConfig.GetStageRect().size.height * 0.5f * ScrollScale
				);
				addChild( background_node, std::numeric_limits<int>::min() );

				mBackgroundNode = background_node;
			}

			//
			// Stage Node
			//
			{
				mStageNode = game::StageNode::create(
					mStageConfig
					, game::StageNode::DebugConfig{ false, false, false, false }
					, step_mole::CircleCollisionComponentConfig{ false, false, false }
					, BulletCachingAmount
				);
				addChild( mStageNode );
			}

			//
			// Player Node
			//
			{
				auto player_node = game::PlayerNode::create(
					1.f
					, game::PlayerNode::DebugConfig{ false }
					, step_mole::CircleCollisionComponentConfig{ false, false, false }
				);
				player_node->setTag( TAG_Player );
				player_node->setCascadeOpacityEnabled( true );
				player_node->setOpacity( 0u );
				player_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				mStageNode->AddPlayer( player_node );
			}

			//
			// Enemy Node
			//
			{
				Vec2 enemy_position = mStageConfig.GetCenter();
				enemy_position.y += ( mStageConfig.GetBulletGenerateRadiusMax() );

				auto enemy_node = game::EnemyNode::create(
					3.f
					, game::EnemyNode::DebugConfig{ false }
					, step_mole::CircleCollisionComponentConfig{ false, false, false }
					, std::bind( &PlayScene::onEnemyProcessEnd, this )
					, std::bind( &game::StageNode::RequestBulletAction, mStageNode, std::placeholders::_1, std::placeholders::_2 )
				);
				enemy_node->setTag( TAG_Enemy );
				enemy_node->setCascadeOpacityEnabled( true );
				enemy_node->setOpacity( 0u );
				enemy_node->setPosition( enemy_position );
				mStageNode->AddEnemy( enemy_node );
			}

			//
			// Center Pivot
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				sprite->setTag( TAG_CenterPivot );
				sprite->setPosition( mStageConfig.GetCenter() );
				sprite->setVisible( false );
				mStageNode->addChild( sprite, std::numeric_limits<int>::min() );
			}

			//
			// Processor
			//
			{
				buildProcessor();
			}

			//
			// Fade In
			//
			{
				auto node = LayerColor::create( Color4B::BLACK );
				node->setTag( TAG_FadeIn );
				addChild( node, std::numeric_limits<int>::max() );
			}

			//
			// Ready
			//
			{
				auto label = Label::createWithTTF( "READY", "fonts/NanumSquareR.ttf", 28 );
				label->setTag( TAG_Ready );
				label->setPosition( visibleCenter );
				label->setOpacity( 0u );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Go
			//
			{
				auto label = Label::createWithTTF( "GO", "fonts/NanumSquareR.ttf", 28 );
				label->setTag( TAG_Go );
				label->setPosition( visibleCenter );
				label->setOpacity( 0u );
				addChild( label, std::numeric_limits<int>::max() );
			}

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( PlayScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void PlayScene::update4Intro( float delta_time )
		{
			switch( mStep )
			{
			case eStep::FadeIn:
			{
				auto fade_out_action = FadeOut::create( 1.8f );
				getChildByTag( TAG_FadeIn )->runAction( fade_out_action );

				++mStep;
			}
			break;
			case eStep::FadeInWait:
				if( 50u > getChildByTag( TAG_FadeIn )->getOpacity() )
				{
					++mStep;
				}
				break;

			case eStep::FadeInPlayer:
			{
				auto action = FadeIn::create( 1.f );
				mStageNode->getChildByTag( TAG_Player )->runAction( action );

				++mStep;
			}
			break;
			case eStep::FadeInPlayerSound:
				if( 30u < mStageNode->getChildByTag( TAG_Player )->getOpacity() )
				{
					experimental::AudioEngine::play2d( "sounds/fx/powerup_003.ogg", false, 0.1f );
					++mStep;
				}
				break;
			case eStep::FadeInPlayerWait:
				if( 200u < mStageNode->getChildByTag( TAG_Player )->getOpacity() )
				{
					++mStep;
				}
				break;

			case eStep::FadeInEnemy:
			{
				auto action = FadeIn::create( 1.f );
				mStageNode->getChildByTag( TAG_Enemy )->runAction( action );

				++mStep;
			}
			break;
			case eStep::FadeInEnemySound:
				if( 30u < mStageNode->getChildByTag( TAG_Player )->getOpacity() )
				{
					experimental::AudioEngine::play2d( "sounds/fx/powerup_003.ogg", false, 0.1f );
					++mStep;
				}
				break;
			case eStep::FadeInEnemyWait:
				if( 200u < mStageNode->getChildByTag( TAG_Enemy )->getOpacity() )
				{
					++mStep;
				}
				break;

			case eStep::EnemyProcessStart:
				startEnemyProcess();
				++mStep;
				break;

			case eStep::Ready:
			{
				auto fade_in_action = FadeIn::create( 0.6f );
				auto delay_action = DelayTime::create( 1.f );
				auto fade_out_action = FadeOut::create( 0.8f );
				auto blinkSequence = Sequence::create( fade_in_action, delay_action, fade_out_action, nullptr );
				getChildByTag( TAG_Ready )->runAction( blinkSequence );

				++mStep;
			}
			break;
			case eStep::ReadyWait_1:
				if( 0u < getChildByTag( TAG_Ready )->getOpacity() )
				{
					++mStep;
				}
				break;
			case eStep::ReadyWait_2:
				if( 0u == getChildByTag( TAG_Ready )->getOpacity() )
				{
					++mStep;
				}
				break;

			case eStep::Go:
			{
				auto fade_in_action = FadeIn::create( 0.6f );
				auto delay_action = DelayTime::create( 1.f );
				auto fade_out_action = FadeOut::create( 0.8f );
				auto blinkSequence = Sequence::create( fade_in_action, delay_action, fade_out_action, nullptr );
				getChildByTag( TAG_Go )->runAction( blinkSequence );

				++mStep;
			}
			break;
			case eStep::GoWait_1:
				if( 100u < getChildByTag( TAG_Go )->getOpacity() )
				{
					schedule( schedule_selector( PlayScene::update4Game ) );
					++mStep;
				}
				break;
			case eStep::GoWait_2:
				if( 0u == getChildByTag( TAG_Go )->getOpacity() )
				{
					++mStep;
				}
				break;

			case eStep::Game:
				unschedule( schedule_selector( PlayScene::update4Intro ) );
				break;

			case eStep::Test:
				getChildByTag( TAG_FadeIn )->setOpacity( 0u );
				mStageNode->getChildByTag( TAG_Enemy )->setOpacity( 255u );
				mStageNode->getChildByTag( TAG_Player )->setOpacity( 255u );
				unschedule( schedule_selector( PlayScene::update4Intro ) );
				schedule( schedule_selector( PlayScene::update4Game ) );
				mPackageIndicator = 18u;
				startEnemyProcess();
				break;
			}
		}
		void PlayScene::update4Game( float delta_time )
		{
			Vec2 move_vector;
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_UP_ARROW ) )
			{
				move_vector.y += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_DOWN_ARROW ) )
			{
				move_vector.y -= 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) )
			{
				move_vector.x += 1.f;
			}
			if( mKeyCodeCollector.isActiveKey( EventKeyboard::KeyCode::KEY_LEFT_ARROW ) )
			{
				move_vector.x -= 1.f;
			}

			if( 0.f != move_vector.x || 0.f != move_vector.y )
			{
				//
				// Update Player Position
				//
				move_vector.normalize();
				move_vector.scale( 150.f * delta_time );
				mStageNode->PlayerMoveRequest( move_vector );

				//
				// Background Scroll
				//
				auto player_node = mStageNode->getChildByTag( TAG_Player );
				auto offset = player_node->getPosition() - mStageConfig.GetStageRect().origin;
				offset.scale( ScrollScale );
				mBackgroundNode->setPosition( -offset );
			}
		}

		void PlayScene::onEnemyProcessEnd()
		{
			++mPackageIndicator;
			startEnemyProcess();
		}
		void PlayScene::startEnemyProcess()
		{
			if( mPackgeContainer.size() <= mPackageIndicator )
			{
				return;
			}

			auto enemy_node = static_cast<game::EnemyNode*>( mStageNode->getChildByTag( TAG_Enemy ) );
			enemy_node->StartProcess( &mPackgeContainer[mPackageIndicator] );
		}

		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_rain_of_chaos::game::TitleScene::create() );
				return;
			}

			mKeyCodeCollector.onKeyPressed( keycode );
		}
		void PlayScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( keycode );
		}


		void PlayScene::buildProcessor()
		{
			auto player_node = mStageNode->getChildByTag( TAG_Player );
			auto enemy_node = static_cast<game::EnemyNode*>( mStageNode->getChildByTag( TAG_Enemy ) );
			auto center_pivot_node = mStageNode->getChildByTag( TAG_CenterPivot );
			float wave_delay = 1.7f;
			bool move_direction = cpg::Random::GetBool();

			game::EnemyNode::EnemyProcessorContainer container;

			// Ready
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 5.5f, move_direction, 360.f ) );

				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 01
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 30.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 1, 1.f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 02
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 30.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 1, 1.f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );


				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 30.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 1, 1.f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );


				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 30.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 1, 1.f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 03
			{
				auto move_processor = game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 1.8f, move_direction, 180.f );
				auto fire_processor = game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 5, 0.3f ) ), enemy_node->GetSpawnInfoContainer() );
				container.emplace_back( game::EnemyProcessor_Tie::Create( mStageConfig, enemy_node, player_node, std::move( move_processor ), std::move( fire_processor ) ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			wave_delay -= 0.1f;

			// Wave 04
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 30.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 45.f, 3, 4, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 05
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 30.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 5, 0.2f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 06
			{
				container.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, enemy_node, player_node, 1.f, 1.1f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );
				container.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, enemy_node, player_node, 0.5f, move_direction, 180.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );
				container.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, enemy_node, player_node, 1.f, 1.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			wave_delay -= 0.1f;
			move_direction = cpg::Random::GetBool();

			// Wave 07
			{
				container.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, enemy_node, player_node, 0.2f, move_direction, 90.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 3, 0.2f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, enemy_node, player_node, 0.2f, move_direction, 90.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, enemy_node, player_node, 0.2f, move_direction, 90.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 3, 0.2f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 08
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 90.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 14.f, 2, 4, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 09
			{
				auto move_processor = game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 1.5f, move_direction, 225.f );
				auto fire_processor = game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 2, 5, 0.3f ) ), enemy_node->GetSpawnInfoContainer() );
				container.emplace_back( game::EnemyProcessor_Tie::Create( mStageConfig, enemy_node, player_node, std::move( move_processor ), std::move( fire_processor ) ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			wave_delay -= 0.1f;
			move_direction = cpg::Random::GetBool();

			// Wave 10
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_01::Create( mStageConfig, enemy_node, player_node, 0.2f, move_direction, 10.f, 40.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 2, 4, 0.2f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );


				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_01::Create( mStageConfig, enemy_node, player_node, 0.2f, move_direction, 10.f, 40.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 2, 4, 0.2f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );


				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_01::Create( mStageConfig, enemy_node, player_node, 0.2f, move_direction, 10.f, 40.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 2, 4, 0.2f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 11
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, enemy_node, player_node, 0.2f, 100.f, 160.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 80.f, 5, 4, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );


				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, enemy_node, player_node, 0.2f, 100.f, 160.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 80.f, 5, 4, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );


				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, enemy_node, player_node, 0.2f, 100.f, 160.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 80.f, 5, 4, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );


				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, enemy_node, player_node, 0.2f, 100.f, 160.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 80.f, 5, 4, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 12
			{
				container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, enemy_node, player_node, 3.f, 250.f, 300.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 70.f, 7, 5, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			wave_delay -= 0.1f;
			move_direction = cpg::Random::GetBool();

			// Wave 13
			{
				for( int i = 0; ; ++i )
				{
					container.emplace_back( game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.3f, move_direction, 20.f ) );
					container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
					container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 50.f, 3, 2, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );

					if( 6 <= i )
					{
						break;
					}

					container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				}


				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 14
			{
				for( int i = 0; ; ++i )
				{
					container.emplace_back( game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, enemy_node, player_node, 0.4f, move_direction, 45.f ) );
					container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
					container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_02_Spread::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, true, 50.f, 7, i + 1, 0.1f, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );

					if( 2 <= i )
					{
						break;
					}

					container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				}


				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 15
			{
				auto move_processor = game::EnemyProcessor_Move_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 3.f, move_direction, 360.f );
				auto fire_processor = game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 13, 0.2f ) ), enemy_node->GetSpawnInfoContainer() );
				container.emplace_back( game::EnemyProcessor_Tie::Create( mStageConfig, enemy_node, player_node, std::move( move_processor ), std::move( fire_processor ) ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			wave_delay -= 0.1f;
			move_direction = cpg::Random::GetBool();

			// Wave 16
			{
				container.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, enemy_node, player_node, 1.f, 1.1f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );
				container.emplace_back( game::EnemyProcessor_Move_Linear_2Target_01::Create( mStageConfig, enemy_node, player_node, 0.5f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );
				container.emplace_back( game::EnemyProcessor_Move_Orbit_01::Create( mStageConfig, enemy_node, player_node, 1.f, 1.f ) );
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 17
			{
				for( int i = 0; ; ++i )
				{
					container.emplace_back( game::EnemyProcessor_Move_CircularSector_Random_02::Create( mStageConfig, enemy_node, player_node, 0.2f, 10.f, 40.f ) );
					container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
					container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, center_pivot_node, std::move( game::SpawnProcessor_MultipleShot_01_CircularSector::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 60.f, 5 + i, 4, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
					
					if( 4 <= i )
					{
						break;
					}

					container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.1f ) );
				}

				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			// Wave 18
			{
				for( int i = 0; ; ++i )
				{
					container.emplace_back( game::EnemyProcessor_Blink_CircularSector_01::Create( mStageConfig, enemy_node, player_node, 0.5f, move_direction, 70.f ) );
					{
						game::SpawnProcessorPackage spawn_processor_package;
						spawn_processor_package.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, 100.f, 6, 2, 0.1f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_Sleep::Create( 0.2f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ false, true }, 52.f, 4, 3, 0.1f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_Sleep::Create( 0.2f ) );
						spawn_processor_package.emplace_back( game::SpawnProcessor_MultipleShot_02_Line::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 14.f, 3, 4, 0.1f ) );

						container.emplace_back( game::EnemyProcessor_Fire_Chain::Create(
							mStageConfig, enemy_node, player_node
							, std::move( spawn_processor_package )
							, enemy_node->GetSpawnInfoContainer()
						) );
					}

					if( 1 <= i )
					{
						break;
					}

					container.emplace_back( game::EnemyProcessor_Sleep::Create( 0.5f ) );
				}

				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}

			wave_delay -= 0.1f;
			move_direction = cpg::Random::GetBool();

			// Wave 19
			{

				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 2, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				
				auto move_processor = game::EnemyProcessor_Move_Linear_01::Create( mStageConfig, enemy_node, player_node, 10.f, move_direction, 45.f );
				auto fire_processor = game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_01::Create( mStageConfig, game::SpawnProcessorConfig{ true, true }, 45, 0.2f ) ), enemy_node->GetSpawnInfoContainer() );
				container.emplace_back( game::EnemyProcessor_Tie::Create( mStageConfig, enemy_node, player_node, std::move( move_processor ), std::move( fire_processor ) ) );

				container.emplace_back( game::EnemyProcessor_Fire_Single::Create( mStageConfig, enemy_node, player_node, std::move( game::SpawnProcessor_SingleShot_02_Spread::Create( mStageConfig, game::SpawnProcessorConfig{ false, false }, true, 50.f, 7, 2, 0.1f, 0.1f ) ), enemy_node->GetSpawnInfoContainer() ) );
				
				container.emplace_back( game::EnemyProcessor_Sleep::Create( wave_delay ) );


				mPackgeContainer.emplace_back( std::move( container ) );
			}
		}
	}
}
