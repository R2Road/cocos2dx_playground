#include "PlayGroundScene.h"

#include <new>
#include <sstream>

#include "step_typetype_RootScene.h"
#include "step_clickclick_RootScene.h"
#include "step_pathfinder_RootScene.h"
#include "step_mole_RootScene.h"
#include "step_rain_of_chaos_RootScene.h"
#include "step_defender_RootScene.h"

#include "ui_practice_RootScene.h"
#include "shader_practice_RootScene.h"
#include "graph_practice_RootScene.h"
#include "input_practice_RootScene.h"

#include "ui_research_RootScene.h"
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
		ss << "[4] : " << step_mole::RootScene::getTitle();
		ss << std::endl;
		ss << "[5] : " << step_rain_of_chaos::RootScene::getTitle();
		ss << std::endl;
		ss << "[6] : " << step_defender::RootScene::getTitle();
		ss << std::endl;
		ss << std::endl;
		ss << "=============================";
		ss << std::endl;
		ss << std::endl;
		ss << "[Q] : " << ui_practice::RootScene::getTitle();
		ss << std::endl;
		ss << "[W] : " << shader_practice::RootScene::getTitle();
		ss << std::endl;
		ss << "[E] : " << graph_practice::RootScene::getTitle();
		ss << std::endl;
		ss << "[R] : " << input_practice::RootScene::getTitle();
		ss << std::endl;
		ss << std::endl;
		ss << "=============================";
		ss << std::endl;
		ss << std::endl;
		ss << "[A] : " << ui_research::RootScene::getTitle();
		ss << std::endl;
		ss << "[S] : " << step99::RootScene::getTitle();

		auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
	getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
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
		Director::getInstance()->replaceScene( step_mole::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_5:
		Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_6:
		Director::getInstance()->replaceScene( step_defender::RootScene::create() );
		break;

	case EventKeyboard::KeyCode::KEY_Q:
		Director::getInstance()->replaceScene( ui_practice::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_W:
		Director::getInstance()->replaceScene( shader_practice::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_E:
		Director::getInstance()->replaceScene( graph_practice::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_R:
		Director::getInstance()->replaceScene( input_practice::RootScene::create() );
		break;

	case EventKeyboard::KeyCode::KEY_A:
		Director::getInstance()->replaceScene( ui_research::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_S:
		Director::getInstance()->replaceScene( step99::RootScene::create() );
		break;

	default:
		CCLOG( "Key Code : %d", keycode );
	}
}
