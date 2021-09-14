#include "step_flipflip_game_test_SelectAndFlipScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"

#include "step_flipflip_game_CardSelectorNode.h"
#include "step_flipflip_game_Constant.h"
#include "step_flipflip_game_StageData.h"
#include "step_flipflip_game_StageViewNode.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game_test
	{
		SelectAndFlipScene::SelectAndFlipScene() : mKeyboardListener( nullptr ), mCardSelectorNode( nullptr ), mStageViewNode( nullptr )
		{}

		Scene* SelectAndFlipScene::create()
		{
			auto ret = new ( std::nothrow ) SelectAndFlipScene();
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

		bool SelectAndFlipScene::init()
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
				ss << "[SpaceBar] : Flip";

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

			const game::StageConfig STAGE_CONFIG{ 6, 3, cocos2d::Size( 40.f, 54.f ) };

			//
			// Stage Data
			//
			game::StageData stage_data;
			stage_data.Reset( STAGE_CONFIG.Width, STAGE_CONFIG.Height, 0 );

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

			return true;
		}

		void SelectAndFlipScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( SelectAndFlipScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void SelectAndFlipScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void SelectAndFlipScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
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
				mStageViewNode->Flip( mCardSelectorNode->GetIndicatorX(), mCardSelectorNode->GetIndicatorY() );
				break;
			}
		}
	}
}
