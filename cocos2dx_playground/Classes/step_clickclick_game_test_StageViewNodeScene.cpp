#include "step_clickclick_game_test_StageViewNodeScene.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "step_clickclick_RootScene.h"

#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageViewNode.h"

USING_NS_CC;

namespace
{
	const int MAX_STAGE_WIDTH = 7;
	const int MAX_STAGE_HEIGHT = 7;
}

namespace step_clickclick
{
	namespace game_test
	{
		StageViewNodeScene::StageViewNodeScene() :
			mKeyboardListener( nullptr )

			, mShuffleCountLabel( nullptr )
			, mStageViewNode( nullptr )
			, mStageSizeLabel( nullptr )
			, mSelectedBlockIndexLabel( nullptr )
			, mShuffleCount( 0 )
			, mCurrentStageWidth( 1 )
			, mCurrentStageHeight( 1 )
		{}

		Scene* StageViewNodeScene::create()
		{
			auto ret = new ( std::nothrow ) StageViewNodeScene();
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

		bool StageViewNodeScene::init()
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
				ss << "[ESC] Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[A/S] Shuffle Count Up/Down";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow R/L] Stage Width Up/Down";
				ss << cpg::linefeed;
				ss << "[Arrow U/D] Stage Height Up/Down";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[SPACE] Setup";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Mouse] Show Block Index";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 0, 61, 33, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			// Center Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( _director->getContentScaleFactor() );
				addChild( pivot, std::numeric_limits<int>::min() );
			}

			//
			// Shuffle Count View
			//
			{
				mShuffleCountLabel = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::RIGHT );
				mShuffleCountLabel->setColor( Color3B::GREEN );
				mShuffleCountLabel->setAnchorPoint( Vec2( 1.f, 1.f ) );
				mShuffleCountLabel->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( mShuffleCountLabel, std::numeric_limits<int>::max() );
			}

			//
			// Stage Size View
			//
			{
				mStageSizeLabel = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::RIGHT );
				mStageSizeLabel->setColor( Color3B::GREEN );
				mStageSizeLabel->setAnchorPoint( Vec2( 1.f, 1.f ) );
				mStageSizeLabel->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height * 0.9f )
				);
				addChild( mStageSizeLabel, std::numeric_limits<int>::max() );
			}

			//
			// Pushed Block Index View
			//
			{
				mSelectedBlockIndexLabel = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 14, Size::ZERO, TextHAlignment::LEFT );
				mSelectedBlockIndexLabel->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				mSelectedBlockIndexLabel->setColor( Color3B::GREEN );
				mSelectedBlockIndexLabel->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height )
				);
				addChild( mSelectedBlockIndexLabel, std::numeric_limits<int>::max() );
			}

			//
			// Stage View
			//
			{
				mStageViewNode = game::StageViewNode::create(
					game::StageViewNode::Config{ true, true }
					, MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT
					, std::bind( &StageViewNodeScene::onBlockSelected, this, std::placeholders::_1 )
				);
				mStageViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mStageViewNode );
			}

			//
			// Setup
			//
			updateShuffleCountView();
			updateStageSizeView();
			onBlockSelected( 0 );

			return true;
		}

		void StageViewNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageViewNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageViewNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void StageViewNodeScene::updateShuffleCountView()
		{
			mShuffleCountLabel->setString( StringUtils::format( "Shuffle Count : %d", mShuffleCount ) );
		}
		void StageViewNodeScene::updateStageSizeView()
		{
			mStageSizeLabel->setString( StringUtils::format( "Stage Size\nW : %d\nH : %d", mCurrentStageWidth, mCurrentStageHeight ) );
		}
		void StageViewNodeScene::onBlockSelected( const int block_linear_index )
		{
			mSelectedBlockIndexLabel->setString( StringUtils::format( "Selected Block : %d", block_linear_index ) );
		}


		void StageViewNodeScene::stageSetup()
		{
			auto stage_data = step_clickclick::game::Stage::create( MAX_STAGE_WIDTH, MAX_STAGE_HEIGHT );
			stage_data->Setup( mCurrentStageWidth, mCurrentStageHeight, mShuffleCount );

			mStageViewNode->Setup( *stage_data );
		}


		void StageViewNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_clickclick::RootScene::create() );
				break;

			case EventKeyboard::KeyCode::KEY_SPACE:
				stageSetup();
				break;

			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: // Increase
				mCurrentStageWidth = std::min( MAX_STAGE_WIDTH, mCurrentStageWidth + 2 );
				updateStageSizeView();
				stageSetup();
				break;
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW: // Decrease
				mCurrentStageWidth = std::max( 1, mCurrentStageWidth - 2 );
				updateStageSizeView();
				stageSetup();
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW: // Increase
				mCurrentStageHeight = std::min( MAX_STAGE_HEIGHT, mCurrentStageHeight + 2 );;
				updateStageSizeView();
				stageSetup();
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW: // Decrease
				mCurrentStageHeight = std::max( 1, mCurrentStageHeight - 2 );
				updateStageSizeView();
				stageSetup();
				break;

			case EventKeyboard::KeyCode::KEY_A:
				++mShuffleCount;
				updateShuffleCountView();
				break;
			case EventKeyboard::KeyCode::KEY_S:
				mShuffleCount = std::max( 0, mShuffleCount - 1 );
				updateShuffleCountView();
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
