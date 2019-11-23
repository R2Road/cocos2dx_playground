#include "RootScene.h"

#include <sstream>

#include "SimpleAudioEngine.h"
#include "Research_KeyboardInputScene.h"

USING_NS_CC;

RootScene::RootScene() : listener( nullptr ) {}

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
	ss << "[1] : Research - Keyboard Input Scene";

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

	listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
	getEventDispatcher()->addEventListenerWithFixedPriority( listener, 1 );
}
void RootScene::onExit()
{
	if( listener )
		getEventDispatcher()->removeEventListener( listener );
	Scene::onExit();
}


void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
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
		Director::getInstance()->replaceScene( Research::KeyboardInputScene::create() );
	}
	break;

	default:
		CCLOG( "Key Code : %d", keycode );
	}
}
