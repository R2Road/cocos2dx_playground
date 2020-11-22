#include "graph_practice_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

#include "graph_practice_BasicScene.h"
#include "graph_practice_Collection01Scene.h"
#include "graph_practice_Collection02Scene.h"
#include "graph_practice_Collection03Scene.h"
#include "graph_practice_Collection04Scene.h"
#include "graph_practice_Collection05Scene.h"
#include "graph_practice_Collection06Scene.h"
#include "PlayGroundScene.h"

USING_NS_CC;

namespace graph_practice
{
	RootScene::RootScene() : mKeyboardListener( nullptr )
	{}

	Scene* RootScene::create()
	{
		auto ret = new ( std::nothrow ) RootScene();
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

	bool RootScene::init()
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
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Playground";
			ss << std::endl;
			ss << std::endl;
			ss << "[SPACE] : " << graph_practice::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[1] : " << graph_practice::Collection01Scene::getTitle();
			ss << std::endl;
			ss << "[2] : " << graph_practice::Collection02Scene::getTitle();
			ss << std::endl;
			ss << "[3] : " << graph_practice::Collection03Scene::getTitle();
			ss << std::endl;
			ss << "[4] : " << graph_practice::Collection04Scene::getTitle();
			ss << std::endl;
			ss << "[5] : " << graph_practice::Collection05Scene::getTitle();
			ss << std::endl;
			ss << "[6] : " << graph_practice::Collection06Scene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 23, 33, 61, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		return true;
	}

	void RootScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
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
			_director->replaceScene( PlayGroundScene::create() );
			return;

		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( graph_practice::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_1:
			_director->replaceScene( graph_practice::Collection01Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( graph_practice::Collection02Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( graph_practice::Collection03Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( graph_practice::Collection04Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( graph_practice::Collection05Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_6:
			_director->replaceScene( graph_practice::Collection06Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
