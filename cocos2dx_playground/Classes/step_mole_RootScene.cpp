#include "step_mole_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "helper_SceneMover.h"

#include "step_mole_animation_ListScene.h"
#include "step_mole_animation_CallbackScene.h"
#include "step_mole_animation_ComponentScene.h"

#include "step_mole_collision_BasicScene.h"
#include "step_mole_collision_ComponentScene.h"

#include "step_mole_game_test_ObjectActionScene.h"
#include "step_mole_game_test_SpawnScene.h"
#include "step_mole_game_test_GroupSpawnScene.h"
#include "step_mole_game_test_HittingNodeScene.h"

#include "step_mole_game_TitleScene.h"

#include "PlayGroundScene.h"

USING_NS_CC;

namespace step_mole
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
			ss << "[1] : " << step_mole::animation::ListScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << step_mole::animation::CallbackScene::getTitle();
			ss << std::endl;
			ss << "[3] : " << step_mole::animation::ComponentScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] : " << step_mole::collision::BasicScene::getTitle();
			ss << std::endl;
			ss << "[5] : " << step_mole::collision::ComponentScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : " << step_mole::game_test::ObjectActionScene::getTitle();
			ss << std::endl;
			ss << "[S] : " << step_mole::game_test::RandomSpawnScene::getTitle();
			ss << std::endl;
			ss << "[D] : " << step_mole::game_test::GroupSpawnScene::getTitle();
			ss << std::endl;
			ss << "[F] : " << step_mole::game_test::HittingNodeScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[SPACE] : " << step_mole::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 0, 9, 61, 255 ) );
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
		getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
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
			Director::getInstance()->replaceScene( step_mole::animation::ListScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( step_mole::animation::CallbackScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( step_mole::animation::ComponentScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( step_mole::collision::BasicScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			Director::getInstance()->replaceScene( step_mole::collision::ComponentScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			Director::getInstance()->replaceScene( step_mole::game_test::ObjectActionScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_S:
			Director::getInstance()->replaceScene( step_mole::game_test::RandomSpawnScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_D:
			Director::getInstance()->replaceScene( step_mole::game_test::GroupSpawnScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_F:
			Director::getInstance()->replaceScene( step_mole::game_test::HittingNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			Director::getInstance()->replaceScene( step_mole::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
