#include "step_flipflip_game_test_CardSelectorNodeScene.h"

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

#include "step_flipflip_game_CardSelectorNode.h"
#include "step_flipflip_game_Constant.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_SelectedIndicatorPosition = 20140416;
}

namespace step_flipflip
{
	namespace game_test
	{
		CardSelectorNodeScene::CardSelectorNodeScene() :
			mKeyboardListener( nullptr )
			, mCardSelectorNode( nullptr )
		{}

		Scene* CardSelectorNodeScene::create()
		{
			auto ret = new ( std::nothrow ) CardSelectorNodeScene();
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

		bool CardSelectorNodeScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();
			const Vec2 visibleCenter(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f ,visibleSize.height * 0.5f )
			);


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
				ss << "[Arrow] : Move Indicator";
				ss << cpg::linefeed;
				ss << "[SpaceBar] : Select";

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
				auto background_layer = LayerColor::create( Color4B( 29, 96, 96, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Selected Indicator Position
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 14 );
				label->setTag( TAG_SelectedIndicatorPosition );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Card Selector Node
			//
			{
				mCardSelectorNode = game::CardSelectorNode::create( game::StageConfig{ 5, 4, cocos2d::Size( 40.f, 54.f ) }, true );
				mCardSelectorNode->setPosition(
					visibleCenter
					- Vec2( mCardSelectorNode->getContentSize().width * 0.5f, mCardSelectorNode->getContentSize().height * 0.5f )
				);
				addChild( mCardSelectorNode );
			}

			//
			// Setup
			//
			updateSelectedIndicatorPositionView();

			return true;
		}

		void CardSelectorNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( CardSelectorNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void CardSelectorNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void CardSelectorNodeScene::updateSelectedIndicatorPositionView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_SelectedIndicatorPosition ) );
			label->setString( StringUtils::format( "Selected - X : %d, Y : %d", mCardSelectorNode->GetIndicatorX(), mCardSelectorNode->GetIndicatorY() ) );
		}


		void CardSelectorNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;

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
				updateSelectedIndicatorPositionView();
				break;
			}
		}
	}
}
