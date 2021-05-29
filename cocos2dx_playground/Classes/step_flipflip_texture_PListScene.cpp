#include "step_flipflip_texture_PListScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "renderer/CCTextureCache.h"

#include "cpg_SStream.h"

#include "step_flipflip_RootScene.h"

USING_NS_CC;

namespace
{
	const char* PLIST_Path = "textures/step_flipflip/step_flipflip_dummy_textures.plist";
	const char* TEXTURE_Path = "textures/step_flipflip/step_flipflip_dummy_textures.png";
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
			// PList Info
			//
			{
				auto label = Label::createWithTTF(
					StringUtils::format( "PList Path : %s\nTexture Path : %s", PLIST_Path, TEXTURE_Path )
					, "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT
				);
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Load PList
			//
			{
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile( PLIST_Path, TEXTURE_Path );
				_director->getTextureCache()->getTextureForKey( TEXTURE_Path )->setAliasTexParameters();
			}

			//
			// Texture View
			//
			{
				auto sprite = Sprite::create( TEXTURE_Path );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.5f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( "Texture 4 PList", "fonts/NanumSquareR.ttf", 12 );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					sprite->getPosition()
					- Vec2( 0.f, sprite->getBoundingBox().size.height * 0.5f )
					- Vec2( 0.f, 20.f )
				);
				addChild( label );
			}

			//
			// Sprite With PList
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_flipflip_dummy4plist_front_0.png" );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.7f, visibleSize.height * 0.5f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( "Sprite With PList", "fonts/NanumSquareR.ttf", 12 );
				label->setColor( Color3B::GREEN );
				label->setPosition(
					sprite->getPosition()
					- Vec2( 0.f, sprite->getBoundingBox().size.height * 0.5f )
					- Vec2( 0.f, 20.f )
				);
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
			_director->getTextureCache()->removeTextureForKey( TEXTURE_Path );

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
