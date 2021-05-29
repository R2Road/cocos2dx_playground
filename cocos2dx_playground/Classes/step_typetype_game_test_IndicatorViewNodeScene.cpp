#include "step_typetype_game_test_IndicatorViewNodeScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_SStream.h"

#include "step_typetype_game_IndicatorViewNode.h"
#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_StageLengthView = 20140416;
	const int TAG_IndicatorPositionView = 20160528;
}

namespace step_typetype
{
	namespace game_test
	{
		IndicatorViewNodeScene::IndicatorViewNodeScene() :
			mKeyboardListener( nullptr )
			, mCurrentStageLength( 4u )
			, mCurrentIndicatorPosition( 0 )
			, mIndicatorViewNode( nullptr )
		{}

		Scene* IndicatorViewNodeScene::create()
		{
			auto ret = new ( std::nothrow ) IndicatorViewNodeScene();
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

		bool IndicatorViewNodeScene::init()
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
				ss << cpg::linefeed;
				ss << "[R] : Stage Reset";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[Arrow L/R] : Move Indicator";

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
				auto layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( layer, std::numeric_limits<int>::min() );
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
			// Indicator View Node
			//
			{
				mIndicatorViewNode = game::IndicatorViewNode::create( game::IndicatorViewNode::Config{ true, true } );
				mIndicatorViewNode->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( mIndicatorViewNode );
			}

			//
			// Setup
			//
			resetIndicatorView();

			return true;
		}

		void IndicatorViewNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( IndicatorViewNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void IndicatorViewNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void IndicatorViewNodeScene::resetIndicatorView()
		{
			mIndicatorViewNode->Reset( mCurrentStageLength );
			mCurrentIndicatorPosition = 0;

			updateStageLengthView();
			updateIndicatorPositionView();
		}
		void IndicatorViewNodeScene::updateStageLengthView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_StageLengthView ) );
			label->setString( StringUtils::format( "Stage Length : %d", mCurrentStageLength ) );
		}
		void IndicatorViewNodeScene::updateIndicatorPositionView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_IndicatorPositionView ) );
			label->setString( StringUtils::format( "Indicator Position : %d", mCurrentIndicatorPosition ) );
		}

		void IndicatorViewNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_typetype::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW: // increase stage size + reset
				++mCurrentStageLength;
				resetIndicatorView();
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW: // decrease stage size + reset
				mCurrentStageLength = (
					mCurrentStageLength > 0u
					? mCurrentStageLength - 1u
					: 0u
				);
				resetIndicatorView();
				break;
				
			case EventKeyboard::KeyCode::KEY_R: // stage reset
				resetIndicatorView();
				break;

			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				--mCurrentIndicatorPosition;
				mIndicatorViewNode->SetIndicatorPosition( mCurrentIndicatorPosition );
				updateIndicatorPositionView();
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				++mCurrentIndicatorPosition;
				mIndicatorViewNode->SetIndicatorPosition( mCurrentIndicatorPosition );
				updateIndicatorPositionView();
				break;
			}
		}
	}
}
