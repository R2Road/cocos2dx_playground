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

#include "graph_practice_test_GraphViewNodeScene.h"
#include "graph_practice_test_GraphAndNameNodeScene.h"
#include "graph_practice_SineScene.h"
#include "graph_practice_CosineScene.h"
#include "graph_practice_TangentScene.h"
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
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[1] : " << graph_practice_test::GraphViewNodeScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << graph_practice_test::GraphAndNameNodeScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] : " << graph_practice::SineScene::getTitle();
			ss << std::endl;
			ss << "[W] : " << graph_practice::CosineScene::getTitle();
			ss << std::endl;
			ss << "[E] : " << graph_practice::TangentScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : " << graph_practice::Collection01Scene::getTitle();
			ss << std::endl;
			ss << "[S] : " << graph_practice::Collection02Scene::getTitle();
			ss << std::endl;
			ss << "[D] : " << graph_practice::Collection03Scene::getTitle();
			ss << std::endl;
			ss << "[F] : " << graph_practice::Collection04Scene::getTitle();
			ss << std::endl;
			ss << "[G] : " << graph_practice::Collection05Scene::getTitle();
			ss << std::endl;
			ss << "[H] : " << graph_practice::Collection06Scene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 11, Size::ZERO, TextHAlignment::LEFT );
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

		case EventKeyboard::KeyCode::KEY_1:
			_director->replaceScene( graph_practice_test::GraphViewNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( graph_practice_test::GraphAndNameNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( graph_practice::SineScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( graph_practice::CosineScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_E:
			_director->replaceScene( graph_practice::TangentScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( graph_practice::Collection01Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_S:
			_director->replaceScene( graph_practice::Collection02Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_D:
			_director->replaceScene( graph_practice::Collection03Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_F:
			_director->replaceScene( graph_practice::Collection04Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_G:
			_director->replaceScene( graph_practice::Collection05Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_H:
			_director->replaceScene( graph_practice::Collection06Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
