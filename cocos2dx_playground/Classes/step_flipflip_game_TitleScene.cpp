#include "step_flipflip_game_TitleScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		TitleScene::TitleScene() : mKeyboardListener( nullptr ), mRequestInputLabel( nullptr ), mElapsedTime( 0.f ) {}

		Scene* TitleScene::create()
		{
			auto ret = new ( std::nothrow ) TitleScene();
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

		bool TitleScene::init()
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
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 8 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}


			//
			// Title
			//
			{
				auto title = Sprite::create( "textures/step_typetype/step_typetype_title.png" );
				title->getTexture()->setAliasTexParameters();
				title->setScaleX( visibleSize.width / title->getContentSize().width );
				title->setScaleY( visibleSize.height / title->getContentSize().height );
				title->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( title, 0 );
			}


			//
			// request input
			//
			{
				mRequestInputLabel = Label::createWithTTF( "PRESS SPACE BAR", "fonts/NanumSquareR.ttf", 14 );
				mRequestInputLabel->setPosition(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.23f
				);
				addChild( mRequestInputLabel, 1 );
			}

			//
			// Setup
			//
			schedule( schedule_selector( TitleScene::update4InputIndicator ) );

			return true;
		}

		void TitleScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TitleScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TitleScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void TitleScene::update4InputIndicator( float dt )
		{
			mElapsedTime += dt;

			if( mRequestInputLabel->isVisible() && mElapsedTime > 0.8f )
			{
				mElapsedTime = 0.f;
				mRequestInputLabel->setVisible( false );
			}
			else if( !mRequestInputLabel->isVisible() && mElapsedTime > 0.4f )
			{
				mElapsedTime = 0.f;
				mRequestInputLabel->setVisible( true );
			}
		}

		void TitleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				//_director->replaceScene( step_flipflip::game::PlayScene::create() );
				return;
			}
		}
	}
}
