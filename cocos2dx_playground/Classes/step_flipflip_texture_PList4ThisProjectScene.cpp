#include "step_flipflip_texture_PList4ThisProjectScene.h"

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
	const char* PLIST_Path = "textures/texture_001.plist";
	const char* TEXTURE_Path = "textures/texture_001.png";
}

namespace step_flipflip
{
	namespace texture
	{
		PList4ThisProjectScene::PList4ThisProjectScene() : mKeyboardListener( nullptr ) {}

		Scene* PList4ThisProjectScene::create()
		{
			auto ret = new ( std::nothrow ) PList4ThisProjectScene();
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

		bool PList4ThisProjectScene::init()
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
			// Texture View
			//
			{
				auto sprite = Sprite::create( TEXTURE_Path );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.5f )
				);
				addChild( sprite );
			}

			//
			// Explain
			//
			{
				auto label = Label::createWithTTF(
					"<=== This texture will be used\nthroughout the project"
					, "fonts/NanumSquareR.ttf", 14, Size::ZERO, TextHAlignment::RIGHT
				);
				label->setColor( Color3B::YELLOW );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.7f, visibleSize.height * 0.5f )
				);
				addChild( label );
			}

			return true;
		}

		void PList4ThisProjectScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PList4ThisProjectScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void PList4ThisProjectScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void PList4ThisProjectScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_flipflip::RootScene::create() );
				return;
			}
		}
	}
}
