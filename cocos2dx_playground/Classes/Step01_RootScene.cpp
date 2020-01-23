#include "Step01_RootScene.h"

#include <new>
#include <sstream>

#include "PlayGroundScene.h"
#include "Step01_Input_RootScene.h"
#include "Step01_Animation_RootScene.h"

USING_NS_CC;

namespace step01
{
	RootScene::RootScene() : mKeyboardListener( nullptr ) {}

	Scene* RootScene::create()
	{
		auto ret = new ( std::nothrow ) RootScene();
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
		}
		else
		{
			ret->autorelease();
		}

		const auto visibleSize = Director::getInstance()->getVisibleSize();
		const auto origin = Director::getInstance()->getVisibleOrigin();

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ Step 01 : Input And Animation";
			ss << "\n";
			ss << "\n";
			ss << "[ESC] : Return to Playground";
			ss << "\n";
			ss << "\n";
			ss << "[1] : Input Research";
			ss << "\n";
			ss << "[2] : Animation Research";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
			label->setPosition( Vec2(
				origin.x + ( visibleSize.width * 0.5f )
				, origin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label, 1 );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 41, 0, 61, 255 ) );
			ret->addChild( background_layer, 0 );
		}

		return ret;
	}

	void RootScene::onEnter()
	{
		Scene::onEnter();

		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
	}
	void RootScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			Director::getInstance()->replaceScene( PlayGroundScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_1:
			Director::getInstance()->replaceScene( input::RootScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( animation::RootScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
