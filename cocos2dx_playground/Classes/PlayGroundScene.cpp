#include "PlayGroundScene.h"

#include <new>
#include <sstream>

#include "cocos2d.h"

#include "cpg_StringTable.h"

#include "step_typetype_RootScene.h"
#include "step_flipflip_RootScene.h"
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

	const auto visibleSize = _director->getVisibleSize();
	const auto visibleOrigin = _director->getVisibleOrigin();

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
		ss << "[2] : " << step_flipflip::RootScene::getTitle();
		ss << std::endl;
		ss << "[3] : " << step_clickclick::RootScene::getTitle();
		ss << std::endl;
		ss << "[4] : " << step_pathfinder::RootScene::getTitle();
		ss << std::endl;
		ss << "[5] : " << step_mole::RootScene::getTitle();
		ss << std::endl;
		ss << "[6] : " << step_rain_of_chaos::RootScene::getTitle();
		ss << std::endl;
		ss << "[7] : " << step_defender::RootScene::getTitle();
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

		auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 11, Size::ZERO, TextHAlignment::LEFT );
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
		_director->end();
		break;

	case EventKeyboard::KeyCode::KEY_1:
		_director->replaceScene( step_typetype::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_2:
		_director->replaceScene( step_flipflip::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_3:
		_director->replaceScene( step_clickclick::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_4:
		_director->replaceScene( step_pathfinder::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_5:
		_director->replaceScene( step_mole::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_6:
		_director->replaceScene( step_rain_of_chaos::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_7:
		_director->replaceScene( step_defender::RootScene::create() );
		break;

	case EventKeyboard::KeyCode::KEY_Q:
		_director->replaceScene( ui_practice::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_W:
		_director->replaceScene( shader_practice::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_E:
		_director->replaceScene( graph_practice::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_R:
		_director->replaceScene( input_practice::RootScene::create() );
		break;

	case EventKeyboard::KeyCode::KEY_A:
		_director->replaceScene( ui_research::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_S:
		_director->replaceScene( step99::RootScene::create() );
		break;

	default:
		CCLOG( "Key Code : %d", keycode );
	}
}
