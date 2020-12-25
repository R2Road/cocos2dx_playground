#include "step_flipflip_game_PlayScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_Random.h"

#include "step_flipflip_game_CardSelectorNode.h"
#include "step_flipflip_game_Constant.h"
#include "step_flipflip_game_StageData.h"
#include "step_flipflip_game_StageViewNode.h"

#include "step_flipflip_game_TitleScene.h"

USING_NS_CC;

namespace
{
	const step_flipflip::game::StageConfig STAGE_CONFIG{ 6, 3, cocos2d::Size( 40.f, 54.f ) };
}

namespace step_flipflip
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mAudioID_forBGM( -1 )

			, mCardSelectorNode( nullptr )
			, mStageViewNode( nullptr )

			, mStep( eStep::Enter )
			, mElapsedTime( 0.f )
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
			const Vec2 visibleCenter(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
			);

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 7 );
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
					"BGM : Somewhere in the Elevator\nAuthor : Peachtea@You're Perfect Studio\nLicense : CC-BY 4.0\nFrom : https://opengameart.org/"
					, "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::RIGHT
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
			// Stage Setup
			//
			game::StageData stage_data;
			stage_data.Reset( STAGE_CONFIG.Width, STAGE_CONFIG.Height );

			//
			// Stage View Node
			//
			{
				mStageViewNode = game::StageViewNode::create( STAGE_CONFIG, stage_data );
				mStageViewNode->setPosition(
					visibleCenter
					- Vec2( mStageViewNode->getContentSize().width * 0.5f, mStageViewNode->getContentSize().height * 0.5f )
				);
				addChild( mStageViewNode );
			}

			//
			// Card Selector Node
			//
			{
				mCardSelectorNode = game::CardSelectorNode::create( STAGE_CONFIG );
				mCardSelectorNode->setPosition(
					visibleCenter
					- Vec2( mCardSelectorNode->getContentSize().width * 0.5f, mCardSelectorNode->getContentSize().height * 0.5f )
				);
				addChild( mCardSelectorNode, 1 );
			}

			//
			// Setup
			//
			mCardSelectorNode->setVisible( false );
			schedule( schedule_selector( PlayScene::Update4GameStart ) );

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

			mAudioID_forBGM = experimental::AudioEngine::play2d( "sounds/bgm/Somewhere_in_the_Elevator.ogg", true, 0.1f );

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PlayScene::onExit()
		{
			experimental::AudioEngine::stop( mAudioID_forBGM );

			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void PlayScene::Update4GameStart( float dt )
		{
			switch( mStep )
			{
			case eStep::Enter:
				++mStep;
				break;

			case eStep::ShowHint:
				for( int current_h = 0; STAGE_CONFIG.Height > current_h; ++current_h )
				{
					mStageViewNode->Flip( cpg::Random::GetInt( 0, STAGE_CONFIG.Width - 1 ), current_h );
				}
				++mStep;
				mElapsedTime = 0.f;
				break;
			case eStep::Sleep4ShowHint:
				mElapsedTime += dt;
				if( 2.f < mElapsedTime )
				{
					++mStep;
					mElapsedTime = 0.f;
				}
				break;

			case eStep::HideHint:
				mStageViewNode->HideAll();
				++mStep;
				break;
			case eStep::Sleep4HideHint:
				mElapsedTime += dt;
				if( 1.f < mElapsedTime )
				{
					++mStep;
					mElapsedTime = 0.f;
				}
				break;

			case eStep::Game:
				mCardSelectorNode->setVisible( true );
				++mStep;
				break;
			}
		}


		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( TitleScene::create() );
				return;
			}

			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				mCardSelectorNode->MoveIndicator( -1, 0 );
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				mCardSelectorNode->MoveIndicator( 1, 0 );
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mCardSelectorNode->MoveIndicator( 0, 1 );
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mCardSelectorNode->MoveIndicator( 0, -1 );
				break;

			case EventKeyboard::KeyCode::KEY_SPACE:
				mStageViewNode->Flip( mCardSelectorNode->GetIndicatorX(), mCardSelectorNode->GetIndicatorY() );
				break;
			}
		}
	}
}
