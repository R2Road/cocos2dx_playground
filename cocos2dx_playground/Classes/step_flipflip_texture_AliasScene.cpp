#include "step_flipflip_texture_AliasScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"

#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace texture
	{
		AliasScene::AliasScene() : mKeyboardListener( nullptr ) {}

		Scene* AliasScene::create()
		{
			auto ret = new ( std::nothrow ) AliasScene();
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

		bool AliasScene::init()
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
			// Sprite : Original
			//
			{
				auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_dummy_01.png" );
				sprite->getTexture()->setAntiAliasTexParameters();
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.25f, visibleSize.height * 0.6f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( "Original", "fonts/NanumSquareR.ttf", 12 );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					sprite->getPosition()
					- Vec2( 0.f, 50.f )
				);
				addChild( label );
			}

			//
			// Sprite : Texture Setting - Antialias
			//
			{
				auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_dummy_01.png" );
				sprite->getTexture()->setAntiAliasTexParameters();
				sprite->setScale( 4.f );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.55f, visibleSize.height * 0.6f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( "Scale x 3\n\nTexture Setting\nAntialias\n\nDefault", "fonts/NanumSquareR.ttf", 12 );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					sprite->getPosition()
					- Vec2( 0.f, 50.f )
				);
				addChild( label );
			}

			//
			// Sprite : Texture Setting - Alias
			//
			{
				auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_dummy_02.png" );
				sprite->getTexture()->setAliasTexParameters();
				sprite->setScale( 4.f );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.8f, visibleSize.height * 0.6f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( "Scale x 3\n\nTexture Setting\nAlias", "fonts/NanumSquareR.ttf", 12 );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					sprite->getPosition()
					- Vec2( 0.f, 50.f )
				);
				addChild( label );
			}

			return true;
		}

		void AliasScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( AliasScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void AliasScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void AliasScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}
		}
	}
}
