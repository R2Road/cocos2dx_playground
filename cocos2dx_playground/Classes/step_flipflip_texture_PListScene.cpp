#include "step_flipflip_texture_PListScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"

#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace
{
	const char* PLIST_Path = "textures/step_flipflip/step_flipflip_textures.plist";
	const char* TEXTURE_Path = "textures/step_flipflip/step_flipflip_textures.png";
}

namespace step_flipflip
{
	namespace texture
	{
		PListScene::PListScene() : mKeyboardListener( nullptr ) {}

		Scene* PListScene::create()
		{
			auto ret = new ( std::nothrow ) PListScene();
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

		bool PListScene::init()
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
			// Load PList
			//
			{
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile( PLIST_Path, TEXTURE_Path );
				_director->getTextureCache()->getTextureForKey( TEXTURE_Path )->setAliasTexParameters();
			}

			//
			// Sprite With PList
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_flipflip_card_front_0.png" );
				sprite->setScale( _director->getContentScaleFactor() );
				sprite->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( sprite );

				auto label = Label::createWithTTF( "PList Texture\nSet Alias ", "fonts/NanumSquareR.ttf", 12 );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					sprite->getPositionX()
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( label );
			}

			return true;
		}

		void PListScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PListScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PListScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile( PLIST_Path );

			Scene::onExit();
		}

		void PListScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}
		}
	}
}
