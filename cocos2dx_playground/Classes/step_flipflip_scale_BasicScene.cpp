#include "step_flipflip_scale_BasicScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_flipflip_game_Constant.h"
#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace
{
	static float ChangeAmount_Per_Seconds = 6.f;
}

namespace step_flipflip
{
	namespace scale
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ), mTestNode( nullptr ), mScaleView( nullptr ), mScaleFlags( 0 ) {}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

		bool BasicScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow R/L] : Scale X - Up/Down";
				ss << std::endl;
				ss << "[Arrow U/D] : Scale Y - Up/Down";

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
			// Scale View
			//
			{
				mScaleView = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 14, Size::ZERO, TextHAlignment::LEFT );
				mScaleView->setAnchorPoint( Vec2( 1.f, 1.f ) );
				mScaleView->setColor( Color3B::GREEN );
				mScaleView->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( mScaleView, std::numeric_limits<int>::max() );
			}

			//
			// Test Node
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_flipflip_card_front_4.png" );
				sprite->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( sprite );

				mTestNode = sprite;
			}

			//
			// Setup
			//
			updateScaleView();
			schedule( schedule_selector( BasicScene::update4Scale ) );

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( BasicScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void BasicScene::update4Scale( float dt )
		{
			if( 0 == mScaleFlags )
			{
				return;
			}

			Vec2 temp;
			if( mScaleFlags & ( 1 << eScaleFlag::Right ) )
			{
				temp.x += ChangeAmount_Per_Seconds;
			}
			if( mScaleFlags & ( 1 << eScaleFlag::Left ) )
			{
				temp.x -= ChangeAmount_Per_Seconds;
			}
			if( mScaleFlags & ( 1 << eScaleFlag::Up ) )
			{
				temp.y += ChangeAmount_Per_Seconds;
			}
			if( mScaleFlags & ( 1 << eScaleFlag::Down ) )
			{
				temp.y -= ChangeAmount_Per_Seconds;
			}

			mTestNode->setScaleX( mTestNode->getScaleX() + ( temp.x * dt ) );
			mTestNode->setScaleY( mTestNode->getScaleY() + ( temp.y * dt ) );
			updateScaleView();
		}

		void BasicScene::updateScaleView()
		{
			mScaleView->setString( StringUtils::format( "X : %.2f\nY : %.2f", mTestNode->getScaleX(), mTestNode->getScaleY() ) );
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}

			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				mScaleFlags |= 1 << eScaleFlag::Right;
				break;
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				mScaleFlags |= 1 << eScaleFlag::Left;
				updateScaleView();
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mScaleFlags |= 1 << eScaleFlag::Up;
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mScaleFlags |= 1 << eScaleFlag::Down;
				break;
			}
		}

		void BasicScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				mScaleFlags ^= 1 << eScaleFlag::Right;
				break;
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				mScaleFlags ^= 1 << eScaleFlag::Left;
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mScaleFlags ^= 1 << eScaleFlag::Up;
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mScaleFlags ^= 1 << eScaleFlag::Down;
				break;
			}
		}
	}
}
