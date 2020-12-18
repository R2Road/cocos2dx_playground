#include "step_flipflip_game_test_SelectAndFlipScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_flipflip_game_CardSelectorNode.h"
#include "step_flipflip_game_Constant.h"
#include "step_flipflip_game_StageViewNode.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game_test
	{
		SelectAndFlipScene::SelectAndFlipScene() : mKeyboardListener( nullptr ), mCardSelectorNode( nullptr )
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
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

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
			// Stage View Node
			//
			{
				auto stage_view_node = game::StageViewNode::create( game::STAGE_CONFIG );
				stage_view_node->setPosition(
					visibleCenter
					- Vec2( stage_view_node->getContentSize().width * 0.5f, stage_view_node->getContentSize().height * 0.5f )
				);
				addChild( stage_view_node );
			}

			//
			// Card Selector Node
			//
			{
				mCardSelectorNode = game::CardSelectorNode::create( game::STAGE_CONFIG.Width, game::STAGE_CONFIG.Height );
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
				mCardSelectorNode->MoveIndicator( -1, 0 );
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				mCardSelectorNode->MoveIndicator( 1, 0 );
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mCardSelectorNode->MoveIndicator( 0, 1 );
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mCardSelectorNode->MoveIndicator( 0, -1 );
				break;

			case EventKeyboard::KeyCode::KEY_SPACE:
				// do something~!
				break;
			}
		}
	}
}
