#include "step_flipflip_game_test_MessageViewNodeScene.h"

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

#include "step_flipflip_game_MessageViewNode.h"
#include "step_flipflip_game_Constant.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game_test
	{
		MessageViewNodeScene::MessageViewNodeScene() :
			mKeyboardListener( nullptr )
			, mMessageViewNode( nullptr )
		{}

		Scene* MessageViewNodeScene::create()
		{
			auto ret = new ( std::nothrow ) MessageViewNodeScene();
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

		bool MessageViewNodeScene::init()
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
				ss << "[Q/W] : Show Message";

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
			// Message View Node
			//
			{
				mMessageViewNode = game::MessageViewNode::create( Color4B::GRAY );
				mMessageViewNode->setPosition( visibleCenter );
				addChild( mMessageViewNode );
			}

			return true;
		}

		void MessageViewNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( MessageViewNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void MessageViewNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void MessageViewNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_Q:
				mMessageViewNode->ShowMessage( "Press Q" );
				break;
			case EventKeyboard::KeyCode::KEY_W:
				mMessageViewNode->ShowMessage( "Press W" );
				break;
			}
		}
	}
}
