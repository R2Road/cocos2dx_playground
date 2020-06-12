#include "step_clickclick_game_StageTestScene.h"

#include <cassert>
#include <functional>
#include <new>
#include <numeric>
#include <sstream>

#include "audio/include/AudioEngine.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_clickclick_RootScene.h"

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
			const int MAX_STAGE_WIDTH = 7;
			const int MAX_STAGE_HEIGHT = 7;

			const int TAG_StageSizeView = 20140416;
			const int TAG_ScoreView = 9999;
		}

		StageTestScene::StageTestScene() :
			mKeyboardListener( nullptr )
			, mStage()
			, mStageView( nullptr )
			, mGridIndexConverter( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT )

			, mScore( 0 )
			, mCurrentStageWidth( 3 )
			, mCurrentStageHeight( 3 )
		{}

		Scene* StageTestScene::create()
		{
			auto ret = new ( std::nothrow ) StageTestScene();
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

		bool StageTestScene::init()
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
				ss << "[Arrow Up] : Increase Stage Size And Reset";
				ss << std::endl;
				ss << "[Arrow Down] : Decrease Stage Size And Reset";
				ss << std::endl;
				ss << std::endl;
				ss << "[Mouse] : Click : Play";
				ss << std::endl;
				ss << std::endl;
				ss << "[R] : Reset";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Stage Length View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 10 );
				label->setTag( TAG_StageSizeView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateStageSizeView();
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
					, std::bind( &StageTestScene::onGameProcess, this, std::placeholders::_1 )
					, StageViewConfig{ true, true }
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
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 10 );
				label->setTag( TAG_ScoreView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + 4.f
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateScoreView();
			}

			return true;
		}

		void StageTestScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageTestScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageTestScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void StageTestScene::onGameProcess( const int block_linear_index )
		{
			Processor::Do( mStage.get(), mStageView, block_linear_index, &mScore );
			updateScoreView();
		}


		void StageTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW: // Increase
				mCurrentStageWidth = (
					MAX_STAGE_WIDTH >= mCurrentStageWidth + 2
					? mCurrentStageWidth + 2
					: MAX_STAGE_WIDTH
				);
				mCurrentStageHeight = (
					MAX_STAGE_WIDTH >= mCurrentStageHeight + 2
					? mCurrentStageHeight + 2
					: MAX_STAGE_HEIGHT
				);
				updateStageSizeView();

				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
				mStageView->Setup( *mStage );
				mScore = 0;
				updateScoreView();
				break;

			case EventKeyboard::KeyCode::KEY_DOWN_ARROW: // Decrease
				mCurrentStageWidth = (
					3 <= mCurrentStageWidth - 2
					? mCurrentStageWidth - 2
					: 3
				);
				mCurrentStageHeight = (
					3 <= mCurrentStageHeight - 2
					? mCurrentStageHeight - 2
					: 3
				);
				updateStageSizeView();

				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
				mStageView->Setup( *mStage );
				mScore = 0;
				updateScoreView();
				break;

			case EventKeyboard::KeyCode::KEY_R: // Reset
				mStage->Setup( mCurrentStageWidth, mCurrentStageHeight );
				mStageView->Setup( *mStage );
				mScore = 0;
				updateScoreView();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}

		void StageTestScene::updateStageSizeView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_StageSizeView ) );
			label->setString( StringUtils::format( "Stage Size    X : %d, Y : %d", mCurrentStageWidth, mCurrentStageHeight ) );
		}

		void StageTestScene::updateScoreView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ScoreView ) );
			label->setString( StringUtils::format( "Score : %d", mScore ) );
		}
	} // namespace game
} // namespace step_clickclick
