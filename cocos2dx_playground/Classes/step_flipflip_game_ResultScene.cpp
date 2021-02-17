#include "step_flipflip_game_ResultScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_flipflip_game_TitleScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		ResultScene::ResultScene() : mKeyboardListener( nullptr ) {}

		Scene* ResultScene::create( const double clear_time, const int failed_count )
		{
			auto ret = new ( std::nothrow ) ResultScene();
			if( !ret || !ret->init( clear_time, failed_count ) )
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

		bool ResultScene::init( const double clear_time, const int failed_count )
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
				ss << "[ESC] : Return to Title";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 6, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::BLACK );
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
				auto background_layer = LayerColor::create( Color4B::WHITE );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Game Clear
			//
			{
				auto label = Label::createWithTTF( "Game Clear", "fonts/NanumSquareR.ttf", 32 );
				label->setColor( Color3B::BLACK );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.7f )
				);
				addChild( label );
			}

			//
			// Result
			//
			{
				auto label = Label::createWithTTF( StringUtils::format( "Clear Time : %.2lf", clear_time ), "fonts/NanumSquareR.ttf", 32 );
				label->setColor( Color3B::BLACK );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
				);
				addChild( label );
			}

			//
			// Failed Count
			//
			{
				auto label = Label::createWithTTF( StringUtils::format( "Failed Count : %d", failed_count ), "fonts/NanumSquareR.ttf", 32 );
				label->setColor( Color3B::BLACK );
				label->setPosition( Vec2(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.3f )
				) );
				addChild( label );
			}

			return true;
		}

		void ResultScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ResultScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ResultScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void ResultScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( TitleScene::create() );
				return;
			}
		}
	}
}
