#include "step_defender_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "PlayGroundScene.h"

#include "step_defender_parallax_node_BasicScene.h"
#include "step_defender_parallax_node_RatioCheckScene.h"
#include "step_defender_chipmunk_BasicScene.h"
#include "step_defender_chipmunk_MoveScene.h"
#include "step_defender_chipmunk_ContactScene.h"

USING_NS_CC;

namespace step_defender
{
	RootScene::RootScene() : mKeyboardListener( nullptr ) {}

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
			ss << "[1] : " << step_defender::parallax_node::BasicScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << step_defender::parallax_node::RatioCheckScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[3] : " << step_defender::chipmunk::BasicScene::getTitle();
			ss << std::endl;
			ss << "[4] : " << step_defender::chipmunk::MoveScene::getTitle();
			ss << std::endl;
			ss << "[5] : " << step_defender::chipmunk::ContactScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 10, 52, 58, 255 ) );
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
			break;

		case EventKeyboard::KeyCode::KEY_1:
			_director->replaceScene( step_defender::parallax_node::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_defender::parallax_node::RatioCheckScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_defender::chipmunk::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( step_defender::chipmunk::MoveScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( step_defender::chipmunk::ContactScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
