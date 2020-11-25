#include "step_typetype_sprite_AliasScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "cocos2d.h"

#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	namespace sprite
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
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
				label->setPosition( Vec2(
					visibleOrigin.x + ( label->getContentSize().width * 0.5f )
					, visibleOrigin.y + visibleSize.height - ( label->getContentSize().height * 0.5f )
				) );
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
			// Sprite : Texture Setting - Antialias
			//
			{
				auto sprite = Sprite::create( "textures/step_flipflip/step_flipflip_dummy_01.png" );
				sprite->getTexture()->setAntiAliasTexParameters();
				sprite->setScale( _director->getContentScaleFactor() );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.5f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( "Texture Setting\nAntialias\n\nDefault", "fonts/NanumSquareR.ttf", 12 );
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
				sprite->setScale( _director->getContentScaleFactor() );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.7f, visibleSize.height * 0.5f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( "Texture Setting\nAlias", "fonts/NanumSquareR.ttf", 12 );
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
