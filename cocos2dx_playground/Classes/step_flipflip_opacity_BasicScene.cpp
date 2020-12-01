#include "step_flipflip_opacity_BasicScene.h"

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
	const float ChangeAmount_Per_Seconds = 100.f;
}

namespace step_flipflip
{
	namespace opacity
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ), mTestNode( nullptr ), mOpacityView( nullptr ), mOpacityFlags( 0 ), mCurrentOpacity( 0.f ) {}

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
				ss << "[Arrow U/D] : Opacity - Up/Down";

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
			// Opacity View
			//
			{
				mOpacityView = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 14, Size::ZERO, TextHAlignment::LEFT );
				mOpacityView->setAnchorPoint( Vec2( 1.f, 1.f ) );
				mOpacityView->setColor( Color3B::GREEN );
				mOpacityView->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( mOpacityView, std::numeric_limits<int>::max() );
			}

			//
			// Test Node
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( GetSpriteFrameName_CardFrontSide( game::eCardType::A ) );
				sprite->setOpacity( GLubyte( 150 ) );
				sprite->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( sprite );

				mTestNode = sprite;
				mCurrentOpacity = sprite->getOpacity();
			}

			//
			// Setup
			//
			updateOpacityView();
			schedule( schedule_selector( BasicScene::update4Opacity ) );

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

		void BasicScene::update4Opacity( float dt )
		{
			if( 0 == mOpacityFlags )
			{
				return;
			}

			float temp_change_amount = 0.f;
			if( mOpacityFlags & ( 1 << eOpacityFlag::Up ) )
			{
				temp_change_amount += ChangeAmount_Per_Seconds;
			}
			if( mOpacityFlags & ( 1 << eOpacityFlag::Down ) )
			{
				temp_change_amount -= ChangeAmount_Per_Seconds;
			}

			mCurrentOpacity = std::max( 0.f, std::min( 255.f, mCurrentOpacity + temp_change_amount * dt ) );

			mTestNode->setOpacity( static_cast<GLubyte>( mCurrentOpacity ) );
			updateOpacityView();
		}

		void BasicScene::updateOpacityView()
		{
			mOpacityView->setString( StringUtils::format( "Opacity : %d / 255", mTestNode->getOpacity() ) );
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
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mOpacityFlags |= 1 << eOpacityFlag::Up;
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mOpacityFlags |= 1 << eOpacityFlag::Down;
				updateOpacityView();
				break;
			}
		}

		void BasicScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mOpacityFlags ^= 1 << eOpacityFlag::Up;
				break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mOpacityFlags ^= 1 << eOpacityFlag::Down;
				break;
			}
		}
	}
}
