#include "graph_practice_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "PlayGroundScene.h"

#include "graph_practice_BasicScene.h"
#include "graph_practice_Collection01Scene.h"
#include "graph_practice_Collection02Scene.h"
#include "graph_practice_Collection03Scene.h"
#include "graph_practice_Collection04Scene.h"
#include "graph_practice_Collection05Scene.h"
#include "graph_practice_Collection06Scene.h"

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

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 23, 33, 61, 255 ) );
			ret->addChild( background_layer, std::numeric_limits<int>::min() );
		}

		return ret;
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
			Director::getInstance()->replaceScene( PlayGroundScene::create() );
			return;

		case EventKeyboard::KeyCode::KEY_SPACE:
			Director::getInstance()->replaceScene( graph_practice::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_1:
			Director::getInstance()->replaceScene( graph_practice::Collection01Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( graph_practice::Collection02Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( graph_practice::Collection03Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( graph_practice::Collection04Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_5:
			Director::getInstance()->replaceScene( graph_practice::Collection05Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_6:
			Director::getInstance()->replaceScene( graph_practice::Collection06Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
