#include "step_clickclick_game_PlayScene.h"

#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "audio/include/AudioEngine.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_StringTable.h"

#include "step_clickclick_game_TitleScene.h"
#include "step_clickclick_game_ResultScene.h"

#include "step_clickclick_game_EffectManagerNode.h"
#include "step_clickclick_game_Processor.h"
#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageViewNode.h"

#include "step_flipflip_game_MessageViewNode.h"

USING_NS_CC;

namespace
{
	const int TAG_ScoreView = 20140416;

	const int MAX_STAGE_WIDTH = 9;
	const int MAX_STAGE_HEIGHT = 9;
}

namespace step_clickclick
{
	namespace game
	{
		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mAudioID_forBGM( -1 )

			, mStage()
			, mStageViewNode( nullptr )
			, mEffectManagerNode( nullptr )
			, mMessageViewNode( nullptr )

			, mScore( 0 )
			, mCurrentStageWidth( 3 )
			, mCurrentStageHeight( 3 )
			
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
				ss << "[ESC] : Return to Title";
				ss << std::endl;
				ss << "[Mouse] : Click";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 6 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
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
			// BGM License
			//
			{
				auto label = Label::createWithTTF(
					"BGM : Somewhere in the Elevator\nAuthor : Peachtea@You're Perfect Studio\nLicense : CC-BY 4.0\nFrom : https://opengameart.org/"
					, cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::RIGHT
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
			// Stage
			//
			{
				mStage = step_clickclick::game::Stage::create( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
			}

			//
			// StageView
			//
			{
				mStageViewNode = step_clickclick::game::StageViewNode::create(
					StageViewNode::Config{ false, false }
					, MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT
					, std::bind( &PlayScene::onGameProcess, this, std::placeholders::_1 )
				);
				mStageViewNode->setPosition( visibleCenter );
				addChild( mStageViewNode );
			}

			//
			// Effect Manager Node
			//
			{
				mEffectManagerNode = game::EffectManagerNode::create( game::EffectManagerNode::Config{ false }, MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
				mEffectManagerNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mEffectManagerNode );
			}

			//
			// Score View
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12 );
				label->setTag( TAG_ScoreView );
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Message View Node
			//
			{
				mMessageViewNode = step_flipflip::game::MessageViewNode::create( Color4B::BLUE );
				mMessageViewNode->setPosition( visibleCenter );
				addChild( mMessageViewNode, std::numeric_limits<int>::max() );
			}

			//
			// Setup
			//
			mStageViewNode->setVisible( false );
			updateScoreView();
			schedule( schedule_selector( PlayScene::updateForNextStep ) );

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


		void PlayScene::onGameProcess( const int block_linear_index )
		{
			Processor::Do( mStage.get(), mStageViewNode, mEffectManagerNode, block_linear_index, &mScore );

			//
			// Score Update
			//
			updateScoreView();

			//
			// Stage Clear
			//
			if( !mStage->HasActiveBlock() )
			{
				mStep = eStep::StageClear;
			}
		}

		void PlayScene::updateScoreView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ScoreView ) );
			label->setString( StringUtils::format( "Score : %4d", mScore ) );
		}

		void PlayScene::updateForNextStep( float dt )
		{
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

			case eStep::ShowLevelIndicator:
				mMessageViewNode->ShowMessage( StringUtils::format( "%d X %d", mCurrentStageWidth, mCurrentStageHeight ).c_str() );
				++mStep;
				break;
			case eStep::Wait4LevelIndicator:
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
			case eStep::StartGame:
				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight, 2 );
				mStageViewNode->Setup( *mStage );
				mStageViewNode->setVisible( true );
				++mStep;
				break;

			//case eStep::PlayGame: break;

			case eStep::StageClear:
				mStageViewNode->setVisible( false );

				mCurrentStageWidth += 2;
				mCurrentStageHeight += 2;
				if( MAX_STAGE_WIDTH >= mCurrentStageWidth )
				{
					mStep = eStep::ShowLevelIndicator;
				}
				else
				{
					++mStep;
				}
				break;

			case eStep::ShowClearIndicator:
				mMessageViewNode->ShowMessage( "Stage Clear" );
				++mStep;
				break;
			case eStep::Wait4ClearIndicator:
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

			case eStep::GameClear:
				_director->replaceScene( step_clickclick::game::ResultScene::create( mScore ) );
				break;
			}
		}

		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_clickclick::game::TitleScene::create() );
				return;
			}
		}
	} // namespace game
} // namespace step_clickclick
