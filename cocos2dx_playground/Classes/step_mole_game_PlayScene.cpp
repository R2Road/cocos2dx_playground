#include "step_mole_game_PlayScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "ui/UIButton.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_mole_game_HittingNode.h"
#include "step_mole_game_StageNode.h"
#include "step_mole_game_TargetManager.h"

#include "step_mole_game_TitleScene.h"
#include "step_mole_game_ResultScene.h"

USING_NS_CC;

namespace
{
	const step_mole::game::StageConfig STAGE_CONFIG{ 7, 5, Size( 40.f, 40.f ) };

	const int TAG_ScoreView = 20140416;
}

namespace step_mole
{
	namespace game
	{
		ProcessAction::ProcessAction( const float life_time, const int spawn_count, const float delay_time, const int repeat_count, const ActionFunc& action_func ) :
			mLifeTime( life_time )
			, mSpawnCount( spawn_count )
			, mDelayTime( delay_time )
			, mElapsedTime( delay_time )
			, mRepeatLimit( repeat_count )
			, mCurrentRepeatCount( 0 )
			, mActionFunc( action_func )
		{}

		ProcessAction::ProcessAction( const float delay_time, const ActionFunc& action_func ) :
			mLifeTime( 0.f )
			, mSpawnCount( 0 )
			, mDelayTime( delay_time )
			, mElapsedTime( 0.f )
			, mRepeatLimit( 0 )
			, mCurrentRepeatCount( 0 )
			, mActionFunc( action_func )
		{}

		bool ProcessAction::Update( float dt )
		{
			mElapsedTime += dt;
			if( mDelayTime <= mElapsedTime )
			{
				mElapsedTime = 0.f;
				++mCurrentRepeatCount;

				mActionFunc( mLifeTime, mSpawnCount );

				return mRepeatLimit > mCurrentRepeatCount;
			}

			return true;
		}


		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mAudioID_forBGM( -1 )
			
			, mTargetManager()
			, mStageView( nullptr )

			, mCurrentSpawnTargetCount( 1 )

			, mProcessActionContainer()
			, mProcessActionIndicator()

			, mScore( 0 )
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

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
					, "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::RIGHT
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
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Score View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 12 );
				label->setTag( TAG_ScoreView );
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height - 10.f
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateScoreView();
			}

			//
			// Target Manager
			//
			{
				mTargetManager = TargetManager::create( STAGE_CONFIG );
			}

			//
			// Stage View
			//
			{
				mStageView = step_mole::game::StageNode::create(
					STAGE_CONFIG
					, std::bind( &TargetManager::ComeHomeTarget, mTargetManager.get(), std::placeholders::_1 )
					, StageNodeConfig{ false, false }
					, CircleCollisionComponentConfig{ false, false, false }
				);
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - mStageView->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - mStageView->getContentSize().height ) * 0.5f )
				) );
				addChild( mStageView );
			}

			//
			// Hitting Node
			//
			{
				auto hitting_node = step_mole::game::HittingNode::create(
					{ STAGE_CONFIG.BlockCount_Horizontal + 1, STAGE_CONFIG.BlockCount_Vercital + 1, STAGE_CONFIG.BlockSize }
					, game::HittingNodeConfig{ false, false }
					, std::bind( &PlayScene::attackProcess, this, std::placeholders::_1, std::placeholders::_2 )
				);
				hitting_node->setPosition( Vec2(
					visibleOrigin.x + ( ( visibleSize.width - hitting_node->getContentSize().width ) * 0.5f )
					, visibleOrigin.y + ( ( visibleSize.height - hitting_node->getContentSize().height ) * 0.5f )
				) );
				addChild( hitting_node, 1 );
			}

			//
			// Process
			//
			{
				const auto delay_func = []( float, int ) {};
				const auto spawn_func = std::bind( &PlayScene::doSpawn, this, std::placeholders::_1, std::placeholders::_2 );
				const auto go_result_func = [this]( float, int ) { Director::getInstance()->replaceScene( step_mole::game::ResultScene::create( mScore ) ); };

				mProcessActionContainer = {
					  ProcessAction( 3.f,	delay_func )
					, ProcessAction( 3,		1,		3.f,	3,		spawn_func )
					, ProcessAction( 3.f,	delay_func )
					, ProcessAction( 3,		1,		0.5f,	2,		spawn_func )
					, ProcessAction( 3.f,	delay_func )
					, ProcessAction( 3,		1,		0.5f,	3,		spawn_func )
					, ProcessAction( 2.f,	delay_func )
					, ProcessAction( 3,		1,		0.5f,	2,		spawn_func )
					, ProcessAction( 1.f,	delay_func )
					, ProcessAction( 3,		1,		0.5f,	4,		spawn_func )
					, ProcessAction( 2.5f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.7f,	4,		spawn_func )
					, ProcessAction( 2.5f,	delay_func )
					, ProcessAction( 2.5f,	3,		0.f,	1,		spawn_func )
					, ProcessAction( 1.f,	delay_func )
					, ProcessAction( 2.5f,	4,		0.f,	1,		spawn_func )
					, ProcessAction( 2.5f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.5f,	5,		spawn_func )
					, ProcessAction( 2.5f,	delay_func )
					, ProcessAction( 3.0f,	6,		0.f,	1,		spawn_func )
					, ProcessAction( 2.f,	delay_func )
					, ProcessAction( 3.0f,	7,		0.f,	1,		spawn_func )
					, ProcessAction( 2.f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.7f,	8,		spawn_func )
					, ProcessAction( 1.f,	delay_func )
					, ProcessAction( 3.0f,	1,		0.1f,	6,		spawn_func )
					, ProcessAction( 2.f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.5f,	8,		spawn_func )
					, ProcessAction( 2.f,	delay_func )
					, ProcessAction( 2.5f,	5,		0.f,	1,		spawn_func )
					, ProcessAction( 2.f,	delay_func )
					, ProcessAction( 2.5f,	6,		0.f,	1,		spawn_func )
					, ProcessAction( 1.5f,	delay_func )
					, ProcessAction( 2.5f,	2,		0.5f,	8,		spawn_func )
					, ProcessAction( 2.f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.2f,	4,		spawn_func )
					, ProcessAction( 1.5f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.3f,	8,		spawn_func )
					, ProcessAction( 1.5f,	delay_func )
					, ProcessAction( 2.5f,	5,		0.f,	1,		spawn_func )
					, ProcessAction( 1.f,	delay_func )
					, ProcessAction( 2.5f,	3,		0.5f,	4,		spawn_func )
					, ProcessAction( 1.5f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.2f,	8,		spawn_func )
					, ProcessAction( 1.f,	delay_func )
					, ProcessAction( 2.5f,	3,		0.2f,	5,		spawn_func )
					, ProcessAction( 1.f,	delay_func )
					, ProcessAction( 2.5f,	1,		0.1f,	14,		spawn_func )
					, ProcessAction( 0.5f,	delay_func )
					, ProcessAction( 2.5f,	6,		0.1f,	3,		spawn_func )
					, ProcessAction( 4.5f,	delay_func )
					, ProcessAction( 3.5f,	35,		0.f,	1,		spawn_func )
					, ProcessAction( 4.5f,	go_result_func )
				};

				mProcessActionIndicator = mProcessActionContainer.begin();
			}

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			mAudioID_forBGM = experimental::AudioEngine::play2d( "sounds/bgm/EmpySpace.ogg", true, 0.1f );

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void PlayScene::onExit()
		{
			experimental::AudioEngine::stop( mAudioID_forBGM );
			mAudioID_forBGM = -1;

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}


		void PlayScene::update( float dt )
		{
			if( mProcessActionContainer.end() != mProcessActionIndicator )
			{
				if( !mProcessActionIndicator->Update( dt ) )
				{
					++mProcessActionIndicator;
				}
			}
			Scene::update( dt );
		}


		void PlayScene::doSpawn( const float life_time, const int spawn_count )
		{
			int target_index = -1;
			for( int i = 0; i < spawn_count; ++i )
			{
				target_index = mTargetManager->GetIdleTarget();
				if( -1 == target_index )
				{
					break;
				}

				mStageView->RequestAction( target_index, life_time );
			}

			experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );
		}
		void PlayScene::attackProcess( const int world_x, const int world_y )
		{
			const auto attack_result = mStageView->RequestAttack( world_x, world_y );
			if( 0 < attack_result )
			{
				experimental::AudioEngine::play2d( "sounds/fx/coin_001.ogg", false, 0.2f );
				CCLOG( "success" );

				mScore += ( attack_result * 100 );
				updateScoreView();
			}
			else // miss callback
			{
				experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );
				CCLOG( "miss" );

				mScore -= 20;
				updateScoreView();
			}
		}


		void PlayScene::updateScoreView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ScoreView ) );
			label->setString( StringUtils::format( "Score : %4d", mScore ) );
		}


		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_mole::game::TitleScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_A:
			{
				const auto target_index = mTargetManager->GetIdleTarget();
				if( -1 != target_index )
				{
					mStageView->RequestAction( target_index, 3.f );
				}

				return;
			}

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
