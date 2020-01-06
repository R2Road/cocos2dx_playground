#include "RootScene.h"

#include <new>
#include <sstream>

#include "SimpleAudioEngine.h"

#include "Research_Input_KeyAllowScene.h"
#include "Research_Input_AllowedKeysTestScene.h"

#include "Research_Input_KeyConfigScene.h"
#include "Research_Input_ConfigedKeysTestScene.h"

USING_NS_CC;

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

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::stringstream ss;
	ss << "+ Root Scene";
	ss << "\n";
	ss << "\n";
	ss << "[ESC] : Shutdown";
	ss << "\n";
	ss << "\n";
	ss << "[1] : Research - Key Allow";
	ss << "\n";
	ss << "[2] : Research - Allowed Keys Test";
	ss << "\n";
	ss << "\n";
	ss << "[3] : Research - Key Config";
	ss << "\n";
	ss << "[4] : Research - Configed Keys Test";

	auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
	label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
	label->setPosition( Vec2(
		origin.x + visibleSize.width / 2
		, origin.y + visibleSize.height
	) );
	ret->addChild( label, 1 );


	ret->autorelease();

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
	if( mKeyboardListener )
		getEventDispatcher()->removeEventListener( mKeyboardListener );
	Scene::onExit();
}


void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
{
	switch( keycode )
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
	{
		cocos2d::Director::getInstance()->end();
	}
	break;

	case EventKeyboard::KeyCode::KEY_1:
	{
		Director::getInstance()->replaceScene( research::input::KeyAllowScene::create() );
	}
	break;
	case EventKeyboard::KeyCode::KEY_2:
	{
		Director::getInstance()->replaceScene( research::input::AllowedKeysTestScene::create() );
	}
	break;

	case EventKeyboard::KeyCode::KEY_3:
	{
		Director::getInstance()->replaceScene( research::input::KeyConfigScene::create() );
	}
	break;
	case EventKeyboard::KeyCode::KEY_4:
	{
		Director::getInstance()->replaceScene( research::input::ConfigedKeysTestScene::create() );
	}
	break;

	default:
		CCLOG( "Key Code : %d", keycode );
	}
}
