#include "Step99_RootScene.h"

#include <new>
#include <sstream>

#include "PlayGroundScene.h"

#include "Research_Input_KeyAllowScene.h"
#include "Research_Input_AllowedKeysTestScene.h"

#include "Research_Input_KeyConfigScene.h"
#include "Research_Input_ConfigedKeysTestScene.h"

USING_NS_CC;

namespace step99
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
		const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << "\n";
			ss << "\n";
			ss << "[ESC] : Return to Playground";
			ss << "\n";
			ss << "\n";
			ss << "[1] : Key Allow";
			ss << "\n";
			ss << "[2] : Allowed Keys Test";
			ss << "\n";
			ss << "\n";
			ss << "[3] : Key Config";
			ss << "\n";
			ss << "[4] : Configed Keys Test";

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label, 1 );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 0, 9, 61, 255 ) );
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
			Director::getInstance()->replaceScene( research::input::KeyAllowScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( research::input::AllowedKeysTestScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( research::input::KeyConfigScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( research::input::ConfigedKeysTestScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
