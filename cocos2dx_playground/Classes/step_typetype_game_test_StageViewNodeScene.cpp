#include "step_typetype_game_test_StageViewNodeScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"

#include "step_typetype_game_Stage.h"
#include "step_typetype_game_StageViewNode.h"
#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_StageLengthView = 20140416;
	const int TAG_IndicatorPositionView = 20160528;

	const int STAGE_MAX_LENGTH = 20;
}

namespace step_typetype
{
	namespace game_test
	{
		StageViewNodeScene::StageViewNodeScene() :
			mKeyboardListener( nullptr )
			, mCurrentStageLength( 4 )
			, mCurrentIndicatorPosition( 0 )
			, mStageViewNode( nullptr )
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
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow U/D] : Stage Size Up/Down And Reset";
				ss << cpg::linefeed;
				ss << "[R] : Stage Reset";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow L/R] : Move Indicator Left/Right";
				ss << cpg::linefeed;
				ss << "[Space] : Letter Die";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Stage Length View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 10 );
				label->setTag( TAG_StageLengthView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Indicator Position View
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 10 );
				label->setTag( TAG_IndicatorPositionView );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2(0.5f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
					+ Vec2( 0.f, 40.f )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Stage View
			//
			{
				mStageViewNode = game::StageViewNode::create( STAGE_MAX_LENGTH, game::StageViewNode::Config{ true } );
				mStageViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mStageViewNode );
			}

			//
			// Setup
			//
			resetStage();
			updateStageLengthView();
			updateIndicatorPositionView();

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

		void StageViewNodeScene::resetStage()
		{
			game::Stage stage( STAGE_MAX_LENGTH );
			stage.Reset( mCurrentStageLength );

			mCurrentIndicatorPosition = stage.GetIndicator_Current();
			updateIndicatorPositionView();

			mStageViewNode->Reset( stage );
		}
		void StageViewNodeScene::updateStageLengthView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_StageLengthView ) );
			label->setString( StringUtils::format( "Stage Length : %d/%d", mCurrentStageLength, STAGE_MAX_LENGTH ) );
		}
		void StageViewNodeScene::updateIndicatorPositionView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_IndicatorPositionView ) );
			label->setString( StringUtils::format( "Indicator Position : %d", mCurrentIndicatorPosition ) );

			if( 0 > mCurrentIndicatorPosition || STAGE_MAX_LENGTH <= mCurrentIndicatorPosition )
			{
				label->setColor( Color3B::RED );
			}
			else
			{
				label->setColor( Color3B::GREEN );
			}
		}

		void StageViewNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_typetype::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW: // increase stage size + reset
				{
					++mCurrentStageLength;
					updateStageLengthView();

					resetStage();
				}
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW: // decrease stage size + reset
				{
					mCurrentStageLength = (
						mCurrentStageLength > 0
						? mCurrentStageLength - 1
						: 0
					);
					updateStageLengthView();

					resetStage();
				}
				break;
				
			case EventKeyboard::KeyCode::KEY_R: // stage reset
				resetStage();
				break;

			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				--mCurrentIndicatorPosition;
				updateIndicatorPositionView();
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				++mCurrentIndicatorPosition;
				updateIndicatorPositionView();
				break;
			case EventKeyboard::KeyCode::KEY_SPACE:
				mStageViewNode->RequestLetterDie( mCurrentIndicatorPosition );
				break;
			}
		}
	}
}
