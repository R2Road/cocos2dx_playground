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
#include "step_flipflip_game_MessageViewNode.h"
#include "step_flipflip_game_StageData.h"
#include "step_flipflip_game_StageViewNode.h"

#include "step_flipflip_game_ResultScene.h"
#include "step_flipflip_game_TitleScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mAudioID_forBGM( -1 )

			, mStageConfig( { 6, 3, cocos2d::Size( 40.f, 54.f ) } )
			, mStageData()
			, mCardSelectorNode( nullptr )
			, mMessageViewNode( nullptr )
			, mStageViewNode( nullptr )

			, mStep( eStep::Enter )
			, mElapsedTime( 0.f )
			, mFlipedCount( 0 )
			, mFlipedPoints()

			, mPlayTime( 0.f )
			, mFailedCount( 0 )
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

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 6 );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
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
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Stage Setup
			//
			mStageData.Reset( mStageConfig.Width, mStageConfig.Height, 1 );

			//
			// Stage View Node
			//
			{
				mStageViewNode = StageViewNode::create( mStageConfig, mStageData );
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
				mCardSelectorNode = CardSelectorNode::create( mStageConfig );
				mCardSelectorNode->setPosition(
					visibleCenter
					- Vec2( mCardSelectorNode->getContentSize().width * 0.5f, mCardSelectorNode->getContentSize().height * 0.5f )
				);
				addChild( mCardSelectorNode, 1 );
			}

			//
			// Message View Node
			//
			{
				mMessageViewNode = MessageViewNode::create( Color4B::BLUE );
				mMessageViewNode->setPosition( visibleCenter );
				addChild( mMessageViewNode, std::numeric_limits<int>::max() );
			}

			//
			// Setup
			//
			mCardSelectorNode->setVisible( false );
			schedule( schedule_selector( PlayScene::update4Game ) );

			return true;
		}

		void PlayScene::onEnter()
		{
			Scene::onEnter();

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


		void PlayScene::update4Game( float dt )
		{
			if( eStep::Game_ShowIndicator <= mStep )
			{
				mPlayTime += dt;
			}

			switch( mStep )
			{
			case eStep::Enter:
				mElapsedTime += dt;
				if( 0.5f < mElapsedTime )
				{
					++mStep;
					mElapsedTime = 0.f;
				}
				break;

			case eStep::Message4Hint:
				mMessageViewNode->ShowMessage( "Hint" );
				++mStep;
				break;
			case eStep::Sleep4Message4Hint:
				if( !mMessageViewNode->isMessaging() )
				{
					mElapsedTime += dt;
					if( 0.5f < mElapsedTime )
					{
						++mStep;
						mElapsedTime = 0.f;
					}
				}
				break;
			case eStep::ShowHint:
				for( int current_h = 0; mStageConfig.Height > current_h; ++current_h )
				{
					mStageViewNode->Flip( cpg::Random::GetInt( 0, mStageConfig.Width - 1 ), current_h );
				}
				++mStep;
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
				if( !mStageViewNode->isFlipping() )
				{
					++mStep;
				}
				break;

			case eStep::Wait4Message4Game:
				mElapsedTime += dt;
				if( 0.5f < mElapsedTime )
				{
					++mStep;
					mElapsedTime = 0.f;
				}
				break;
			case eStep::Message4Game:
				mMessageViewNode->ShowMessage( "Find The Same Card Pair" );
				++mStep;
				break;
			case eStep::Sleep4Message4Game:
				if( !mMessageViewNode->isMessaging() )
				{
					mElapsedTime += dt;
					if( 0.5f < mElapsedTime )
					{
						++mStep;
						mElapsedTime = 0.f;
					}
				}
				break;
			case eStep::BGM_Start:
				mAudioID_forBGM = experimental::AudioEngine::play2d( "sounds/bgm/Somewhere_in_the_Elevator.ogg", true, 0.1f );
				++mStep;
				break;

			case eStep::Game_ShowIndicator:
				mCardSelectorNode->setVisible( true );
				mStep = eStep::Game_SelectCard;
				break;
			//case eStep::Game_SelectCard:
			case eStep::Game_HideIndicator:
				mCardSelectorNode->setVisible( false );
				++mStep;
				break;
			case eStep::Game_Wait4DecideCard:
				if( !mStageViewNode->isFlipping() )
				{
					++mStep;
				}
				break;
			case eStep::Game_DecideCard:
				mElapsedTime += dt;
				if( 0.3f < mElapsedTime )
				{
					mElapsedTime = 0.f;

					mFlipedCount = 0;
					if( mStageData.GetType( mFlipedPoints[0].x, mFlipedPoints[0].y ) == mStageData.GetType( mFlipedPoints[1].x, mFlipedPoints[1].y ) )
					{
						mStep = eStep::Game_SelectSuccess;
					}
					else
					{
						mStep = eStep::Game_SelectFailed;
						++mFailedCount;
					}
				}
				break;
			case eStep::Game_SelectFailed:
				experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );
				for( auto& p : mFlipedPoints )
				{
					mStageData.SetStatus( eCardStatus::Close, p.x, p.y );
					mStageViewNode->Flip( p.x, p.y );
				}
				mStep = eStep::Game_ClearCheck;
				break;
			case eStep::Game_SelectSuccess:
				experimental::AudioEngine::play2d( "sounds/fx/coin_001.ogg", false, 0.2f );
				mStep = eStep::Game_ClearCheck;
				break;
			case eStep::Game_ClearCheck:
				if( 0 < mStageData.GetClosedCardsCount() )
				{
					mStep = eStep::Game_ShowIndicator;
				}
				else
				{
					mStep = eStep::Game_Result;
				}
				break;

			case eStep::Game_Result:
				_director->replaceScene( ResultScene::create( mPlayTime, mFailedCount ) );
				return;
			}
		}


		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( TitleScene::create() );
				return;
			}

			if( eStep::Game_SelectCard == mStep )
			{
				switch( keycode )
				{
				case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
					mCardSelectorNode->MoveIndicator( -1, 0, true );
					break;
				case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
					mCardSelectorNode->MoveIndicator( 1, 0, true );
					break;
				case EventKeyboard::KeyCode::KEY_UP_ARROW:
					mCardSelectorNode->MoveIndicator( 0, 1, true );
					break;
				case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
					mCardSelectorNode->MoveIndicator( 0, -1, true );
					break;

				case EventKeyboard::KeyCode::KEY_SPACE:
					if( eCardStatus::Close == mStageData.GetStatus( mCardSelectorNode->GetIndicatorX(), mCardSelectorNode->GetIndicatorY() ) )
					{
						mStageData.SetStatus( eCardStatus::Open, mCardSelectorNode->GetIndicatorX(), mCardSelectorNode->GetIndicatorY() );

						mStageViewNode->Flip( mCardSelectorNode->GetIndicatorX(), mCardSelectorNode->GetIndicatorY() );
						mFlipedPoints[mFlipedCount] = { mCardSelectorNode->GetIndicatorX(), mCardSelectorNode->GetIndicatorY() };
						++mFlipedCount;
						if( 2 <= mFlipedCount )
						{
							mStep = eStep::Game_HideIndicator;
						}
					}
					else
					{
						experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );
					}
					CCLOG( "Closed Cards Count : %d", mStageData.GetClosedCardsCount() );
					break;
				}
			}
		}
	}
}
