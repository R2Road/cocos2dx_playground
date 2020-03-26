#include "PlayGroundScene.h"

#include <new>
#include <sstream>

#include "step_typetype_RootScene.h"
#include "step_clickclick_RootScene.h"
#include "step_pathfinder_RootScene.h"
#include "step_rain_of_chaos_RootScene.h"

#include "Research_Input_GamePadTest.h"
#include "Step99_RootScene.h"

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
	else
	{
		ret->autorelease();
	}

	return ret;
}

bool PlayGroundScene::init()
{
	if( !Scene::init() )
	{
		return false;
	}

	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

	//
	// Summury
	//
	{
		std::stringstream ss;
		ss << "+ Playground";
		ss << std::endl;
		ss << std::endl;
		ss << "[ESC] : Shutdown";
		ss << std::endl;
		ss << std::endl;
		ss << "[1] : " << step_typetype::RootScene::getTitle();
		ss << std::endl;
		ss << "[2] : " << step_clickclick::RootScene::getTitle();
		ss << std::endl;
		ss << "[3] : " << step_pathfinder::RootScene::getTitle();
		ss << std::endl;
		ss << "[4] : " << step_rain_of_chaos::RootScene::getTitle();
		ss << std::endl;
		ss << std::endl;
		ss << std::endl;
		ss << std::endl;
		ss << "[8] : " << research::input::GamePadTestScene::getTitle();
		ss << std::endl;
		ss << "[9] : " << step99::RootScene::getTitle();

		auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
		label->setPosition( Vec2(
			visibleOrigin.x + ( visibleSize.width * 0.5f )
			, visibleOrigin.y + ( visibleSize.height * 0.5f )
		) );
		addChild( label );
	}

	return true;
}

void PlayGroundScene::onEnter()
{
	Scene::onEnter();

	assert( !mKeyboardListener );
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
		Director::getInstance()->replaceScene( step_typetype::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_2:
		Director::getInstance()->replaceScene( step_clickclick::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_3:
		Director::getInstance()->replaceScene( step_pathfinder::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_4:
		Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
		break;

	case EventKeyboard::KeyCode::KEY_8:
		Director::getInstance()->replaceScene( research::input::GamePadTestScene::create() );
		break;

	case EventKeyboard::KeyCode::KEY_9:
		Director::getInstance()->replaceScene( step99::RootScene::create() );
		break;

	default:
		CCLOG( "Key Code : %d", keycode );
	}
}
