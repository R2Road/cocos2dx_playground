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

#include "step_clickclick_game_TitleScene.h"
#include "step_clickclick_game_ResultScene.h"

#include "step_clickclick_game_Processor.h"
#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
			const int TAG_ScoreView = 20140416;
			const int TAG_ClearView = 20160528;
			const int TAG_CountView = 9999;

			const int MAX_STAGE_WIDTH = 9;
			const int MAX_STAGE_HEIGHT = 9;
		}

		PlayScene::PlayScene() :
			mKeyboardListener( nullptr )
			, mAudioID_forBGM( -1 )

			, mStage()
			, mStageView( nullptr )

			, mScore( 0 )
			, mCurrentStageWidth( 3 )
			, mCurrentStageHeight( 3 )
			, mNextStepData()
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
				ss << std::endl;
				ss << "[Mouse] : Click";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 8 );
				label->setColor( Color3B::WHITE );
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
			// Stage
			//
			{
				mStage = step_clickclick::game::Stage::create( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
			}

			//
			// StageView
			//
			{
				mStageView = step_clickclick::game::StageView::create(
					MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT
					, std::bind( &PlayScene::onGameProcess, this, std::placeholders::_1 )
					, StageViewConfig{ false, false }
				);
				mStageView->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( mStageView );

				mStageView->Setup( *mStage );
			}

			//
			// Score View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12 );
				label->setTag( TAG_ScoreView );
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateScoreView();
			}

			//
			// Clear View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 16 );
				label->setTag( TAG_ClearView );
				label->setVisible( false );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.6f
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Count View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 16 );
				label->setTag( TAG_CountView );
				label->setVisible( false );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.4f
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateCountView( mNextStepData.LimitTime_forCount );
			}

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
			Processor::Do( mStage.get(), mStageView, block_linear_index, &mScore );

			//
			// Score Update
			//
			updateScoreView();

			//
			// Stage Clear
			//
			if( !mStage->HasActiveBlock() )
			{
				experimental::AudioEngine::play2d( "sounds/fx/powerup_001.ogg", false, 0.1f );
				schedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
			}
		}

		void PlayScene::updateScoreView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ScoreView ) );
			label->setString( StringUtils::format( "Score : %4d", mScore ) );
		}
		void PlayScene::updateCountView( const float count )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_CountView ) );
			label->setString( StringUtils::format( "%.1f", count ) );
		}

		void PlayScene::updateForNextStep( float dt )
		{
			switch( mNextStepData.Step )
			{
			case NextStepData::eStep::wait_for_entry:
				mNextStepData.ElapsedTime_forEntry += dt;
				if( mNextStepData.LimitTime_forEntry < mNextStepData.ElapsedTime_forEntry )
				{
					mStageView->setVisible( false );
					mNextStepData.ElapsedTime_forEntry = 0.f;

					++mNextStepData.Step;
				}
				break;
			case NextStepData::eStep::show_clear_indicator:
			{
				auto clear_view_label = static_cast<Label*>( getChildByTag( TAG_ClearView ) );
				clear_view_label->setString( "Stage Clear" );
				clear_view_label->setVisible( true );

				auto count_view_label = static_cast<Label*>( getChildByTag( TAG_CountView ) );
				count_view_label->setVisible( true );
				updateCountView( mNextStepData.LimitTime_forCount );

				mCurrentStageWidth += 2;
				mCurrentStageHeight += 2;
				if( MAX_STAGE_WIDTH >= mCurrentStageWidth )
				{
					++mNextStepData.Step;
				}
				else
				{
					mNextStepData.Step = NextStepData::eStep::game_clear;
				}
			}
			break;
			case NextStepData::eStep::wait_for_count:
				mNextStepData.ElapsedTime_forCount += dt;
				if( mNextStepData.LimitTime_forCount < mNextStepData.ElapsedTime_forCount )
				{
					mNextStepData.ElapsedTime_forCount = 0.f;
					updateCountView( 0.f );

					++mNextStepData.Step;
				}
				else
				{
					updateCountView( mNextStepData.LimitTime_forCount - mNextStepData.ElapsedTime_forCount );
				}
				break;
			case NextStepData::eStep::hide_clear_indicator:
				getChildByTag( TAG_ClearView )->setVisible( false );
				getChildByTag( TAG_CountView )->setVisible( false );
				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
				mStageView->Setup( *mStage );
				++mNextStepData.Step;
				break;
			case NextStepData::eStep::reset:
				mStageView->setVisible( true );
				unschedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
				mNextStepData.Step = NextStepData::eStep::wait_for_entry;
				break;

			case NextStepData::eStep::game_clear:
				unschedule( SEL_SCHEDULE( &PlayScene::updateForNextStep ) );
				Director::getInstance()->replaceScene( step_clickclick::game::ResultScene::create( mScore ) );
				break;

			default:
				assert( false );
			}
		}

		void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				return;
			}

			Director::getInstance()->replaceScene( step_clickclick::game::TitleScene::create() );
		}
	} // namespace game
} // namespace step_clickclick
