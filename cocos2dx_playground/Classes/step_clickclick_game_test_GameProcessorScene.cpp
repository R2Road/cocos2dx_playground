#include "step_clickclick_game_test_GameProcessorScene.h"

#include <cassert>
#include <functional>
#include <new>
#include <numeric>

#include "audio/include/AudioEngine.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_EffectManagerNode.h"
#include "step_clickclick_game_Processor.h"
#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageViewNode.h"

USING_NS_CC;

namespace
{
	const int MAX_STAGE_WIDTH = 7;
	const int MAX_STAGE_HEIGHT = 7;

	const int TAG_ScoreView = 9999;
}

namespace step_clickclick
{
	namespace game_test
	{
		GameProcessorScene::GameProcessorScene() :
			mKeyboardListener( nullptr )
			, mStage()
			, mStageViewNode( nullptr )
			, mEffectManagerNode( nullptr )
			, mGridIndexConverter( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT )

			, mScore( 0 )
		{}

		Scene* GameProcessorScene::create()
		{
			auto ret = new ( std::nothrow ) GameProcessorScene();
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

		bool GameProcessorScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Mouse] : Click : Play";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[R] : Reset";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Stage
			//
			{
				mStage = step_clickclick::game::Stage::create( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
				mStage->Setup( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT, 2 );
			}

			//
			// StageView
			//
			{
				mStageViewNode = step_clickclick::game::StageViewNode::create(
					game::StageViewNode::Config{ true, true }
					, MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT
					, std::bind( &GameProcessorScene::onGameProcess, this, std::placeholders::_1 )
				);
				mStageViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
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
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Setup
			//
			mStageViewNode->Setup( *mStage );
			updateScoreView();

			return true;
		}

		void GameProcessorScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GameProcessorScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void GameProcessorScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void GameProcessorScene::onGameProcess( const int block_linear_index )
		{
			game::Processor::Do( mStage.get(), mStageViewNode, mEffectManagerNode, block_linear_index, &mScore );
			updateScoreView();
		}

		void GameProcessorScene::updateScoreView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ScoreView ) );
			label->setString( StringUtils::format( "Score : %d", mScore ) );
		}


		void GameProcessorScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_R: // Reset
				mStage->Setup( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT, 2 );
				mStageViewNode->Setup( *mStage );
				mScore = 0;
				updateScoreView();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
