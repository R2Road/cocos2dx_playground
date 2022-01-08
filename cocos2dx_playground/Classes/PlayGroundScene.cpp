#include "PlayGroundScene.h"

#include <new>

#include "cocos2d.h"

#include "cpg_SStream.h"
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
#include "tool_practice_RootScene.h"
#include "algorithm_practice_RootScene.h"

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
	const Vec2 visibleCenter(
		visibleOrigin.x + ( visibleSize.width * 0.5f )
		, visibleOrigin.y + ( visibleSize.height * 0.5f )
	);

	//
	// Message
	//
	{
		std::stringstream ss;
		ss << "# Message" << cpg::linefeed;
		ss << "> InProgress : Particle" << cpg::linefeed;
		ss << "> InProgress : Algorithm : LoopHero" << cpg::linefeed;
		ss << "> To Do : UI Research : Option Bar Node" << cpg::linefeed;
		ss << "> To Do : Step 07" << cpg::linefeed;
		ss << "> To Do : Step 01 Update" << cpg::linefeed;

		auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 7 );
		label->setAnchorPoint( Vec2( 0.f, 1.f ) );
		label->setPositionY( visibleOrigin.y + visibleSize.height );
		addChild( label );
	}

	//
	// Summury
	//
	{
		std::stringstream ss;
		ss << "+ Playground";
		ss << cpg::linefeed;
		ss << cpg::linefeed;
		ss << "[ESC] : Shutdown";
		ss << cpg::linefeed;
		ss << cpg::linefeed;
		ss << "[1] : " << step_typetype::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[2] : " << step_flipflip::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[3] : " << step_clickclick::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[4] : " << step_pathfinder::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[5] : " << step_mole::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[6] : " << step_rain_of_chaos::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[7] : " << step_defender::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << cpg::linefeed;
		ss << "=============================";
		ss << cpg::linefeed;
		ss << cpg::linefeed;
		ss << "[Q] : " << ui_practice::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[W] : " << shader_practice::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[E] : " << graph_practice::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[R] : " << input_practice::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[T] : " << tool_practice::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[Y] : " << algorithm_practice::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << cpg::linefeed;
		ss << "=============================";
		ss << cpg::linefeed;
		ss << cpg::linefeed;
		ss << "[A] : " << ui_research::RootScene::getTitle();
		ss << cpg::linefeed;
		ss << "[S] : " << step99::RootScene::getTitle();

		auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 8 );
		label->setPosition( visibleCenter );
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
	case EventKeyboard::KeyCode::KEY_T:
		_director->replaceScene( tool_practice::RootScene::create() );
		break;
	case EventKeyboard::KeyCode::KEY_Y:
		_director->replaceScene( algorithm_practice::RootScene::create() );
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
