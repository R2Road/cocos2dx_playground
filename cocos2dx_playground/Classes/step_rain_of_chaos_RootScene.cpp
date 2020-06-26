#include "step_rain_of_chaos_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "PlayGroundScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollectScene.h"
#include "step_rain_of_chaos_batch_node_BasicScene.h"
#include "step_rain_of_chaos_batch_node_QuadScene.h"
#include "step_rain_of_chaos_collision_CollectionScene.h"

#include "step02_game_rain_of_chaos_TitleScene.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	RootScene::RootScene() : mKeyboardListener( nullptr ) {}

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
			ss << "[1] : " << step_rain_of_chaos::input::KeyCodeCollectScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[2] : " << step_rain_of_chaos::batch_node::BasicScene::getTitle();
			ss << std::endl;
			ss << "[3] : " << step_rain_of_chaos::batch_node::QuadScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] : " << step_rain_of_chaos::collision::CollectionScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[SPACE] : " << step02::game::rain_of_chaos::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 145, 55, 33, 255 ) );
			ret->addChild( background_layer, -1 );
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
			break;

		case EventKeyboard::KeyCode::KEY_1:
			Director::getInstance()->replaceScene( step_rain_of_chaos::input::KeyCodeCollectScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( step_rain_of_chaos::batch_node::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( step_rain_of_chaos::batch_node::QuadScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( step_rain_of_chaos::collision::CollectionScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			Director::getInstance()->replaceScene( step02::game::rain_of_chaos::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
