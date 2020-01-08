#include "PlayGroundScene.h"

#include <new>
#include <sstream>

#include "Research_Input_RootScene.h"

USING_NS_CC;

PlayGroundScene::PlayGroundScene() : mKeyboardListener( nullptr ) {}

Scene* PlayGroundScene::create()
{
	auto ret = new ( std::nothrow ) PlayGroundScene();
	if( !ret || !ret->init() )
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::stringstream ss;
	ss << "+ Playground";
	ss << "\n";
	ss << "\n";
	ss << "[ESC] : Shutdown";
	ss << "\n";
	ss << "\n";
	ss << "[1] : Input Research";

	auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
	label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
	label->setPosition( Vec2(
		origin.x + ( visibleSize.width * 0.5f )
		, origin.y + ( visibleSize.height * 0.5f )
	) );
	ret->addChild( label, 1 );


	ret->autorelease();

	return ret;
}

void PlayGroundScene::onEnter()
{
	Scene::onEnter();

	mKeyboardListener = EventListenerKeyboard::create();
	mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayGroundScene::onKeyPressed, this );
	getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
}
void PlayGroundScene::onExit()
{
	assert( mKeyboardListener );
	getEventDispatcher()->removeEventListener( mKeyboardListener );
	mKeyboardListener = nullptr;

	Scene::onExit();
}


void PlayGroundScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
{
	switch( keycode )
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		cocos2d::Director::getInstance()->end();
		break;

	case EventKeyboard::KeyCode::KEY_1:
		Director::getInstance()->replaceScene( research::input::RootScene::create() );
		break;

	default:
		CCLOG( "Key Code : %d", keycode );
	}
}