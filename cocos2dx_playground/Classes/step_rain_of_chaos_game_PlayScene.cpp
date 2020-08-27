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
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_rain_of_chaos_game_BackgroundNode.h"
#include "step_rain_of_chaos_game_EnemyNode.h"
#include "step_rain_of_chaos_game_PlayerNode.h"
#include "step_rain_of_chaos_game_StageNode.h"

#include "step_rain_of_chaos_game_TitleScene.h"

USING_NS_CC;

namespace
{
	const int BulletCachingAmount = 100;

	const int TAG_FadeIn = 10001;
	const int TAG_Ready = 10002;
	const int TAG_Go = 10003;
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

			, mStep( eStep::FadeIn )
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

			schedule( schedule_selector( PlayScene::Update4Game) );

			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
			const auto visibleSize = Director::getInstance()->getVisibleSize();
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

			//
			// Background Node
			//
			{
				const auto tile_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_0.png" )->getOriginalSizeInPixels();

				const auto div_result_width = std::div( static_cast<int>( visibleSize.width ), static_cast<int>( tile_size.width ) );
				const std::size_t vertical_amount = div_result_width.rem > 0 ? div_result_width.quot + 1 : div_result_width.quot;

				const auto div_result_height = std::div( static_cast<int>( visibleSize.height ), static_cast<int>( tile_size.height ) );
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
					visibleOrigin.x + ( visibleSize.width * 0.5f ) - ( background_node->getContentSize().width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f ) - ( background_node->getContentSize().height * 0.5f )
				);
				addChild( background_node, std::numeric_limits<int>::min() );
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
					, std::bind( &game::StageNode::RequestBulletAction, mStageNode, std::placeholders::_1, std::placeholders::_2 )
				);
				enemy_node->setPosition( enemy_position );
				mStageNode->AddEnemy( enemy_node );
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

		void PlayScene::Update4Game( float delta_time )
		{
			switch( mStep )
			{
			case eStep::FadeIn:
			{
				auto fade_out_action = FadeOut::create( 1.8f );
				getChildByTag( TAG_FadeIn )->runAction( fade_out_action );

				mStep = eStep::FadeInWait;
			}
			break;
			case eStep::FadeInWait:
				if( 0u == getChildByTag( TAG_FadeIn )->getOpacity() )
				{
					mStep = eStep::Ready;
				}
				break;

			case eStep::Ready:
			{
				auto fade_in_action = FadeIn::create( 0.6f );
				auto delay_action = DelayTime::create( 1.f );
				auto fade_out_action = FadeOut::create( 0.8f );
				auto blinkSequence = Sequence::create( fade_in_action, delay_action, fade_out_action, nullptr );
				getChildByTag( TAG_Ready )->runAction( blinkSequence );

				mStep = eStep::ReadyWait_1;
			}
			break;
			case eStep::ReadyWait_1:
				if( 0u < getChildByTag( TAG_Ready )->getOpacity() )
				{
					mStep = eStep::ReadyWait_2;
				}
				break;
			case eStep::ReadyWait_2:
				if( 0u == getChildByTag( TAG_Ready )->getOpacity() )
				{
					mStep = eStep::Go;
				}
				break;

			case eStep::Go:
			{
				auto fade_in_action = FadeIn::create( 0.6f );
				auto delay_action = DelayTime::create( 1.f );
				auto fade_out_action = FadeOut::create( 0.8f );
				auto blinkSequence = Sequence::create( fade_in_action, delay_action, fade_out_action, nullptr );
				getChildByTag( TAG_Go )->runAction( blinkSequence );

				mStep = eStep::GoWait_1;
			}
			break;
			case eStep::GoWait_1:
				if( 0u < getChildByTag( TAG_Go )->getOpacity() )
				{
					mStep = eStep::GoWait_2;
				}
				break;
			case eStep::GoWait_2:
				if( 0u == getChildByTag( TAG_Go )->getOpacity() )
				{
					mStep = eStep::Game;
				}
				break;

			case eStep::Game:
				UpdateForInput( delta_time );
				break;

			case eStep::GameOver:
				break;
			}
		}
		void PlayScene::UpdateForInput( float delta_time )
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
				move_vector.normalize();
				move_vector.scale( 100.f * delta_time );

				mStageNode->PlayerMoveRequest( move_vector );
			}
		}

		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_rain_of_chaos::game::TitleScene::create() );
				return;
			}

			mKeyCodeCollector.onKeyPressed( keycode );
		}
		void PlayScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( keycode );
		}
	}
}
