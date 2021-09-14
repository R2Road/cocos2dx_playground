#include "step_mole_RootScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "helper_SceneMover.h"

#include "step_mole_animation_CallbackScene.h"
#include "step_mole_animation_ComponentScene.h"

#include "step_mole_collision_BasicScene.h"
#include "step_mole_collision_ComponentScene.h"

#include "step_mole_game_test_ObjectActionScene.h"
#include "step_mole_game_test_SpawnScene.h"
#include "step_mole_game_test_HittingNodeScene.h"
#include "step_mole_game_test_SpawnAndHitScene.h"

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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Playground";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[1] : " << step_mole::animation::CallbackScene::getTitle();
			ss << cpg::linefeed;
			ss << "[2] : " << step_mole::animation::ComponentScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[3] : " << step_mole::collision::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << "[4] : " << step_mole::collision::ComponentScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[A] : " << step_mole::game_test::ObjectActionScene::getTitle();
			ss << cpg::linefeed;
			ss << "[S] : " << step_mole::game_test::SpawnScene::getTitle();
			ss << cpg::linefeed;
			ss << "[D] : " << step_mole::game_test::HittingNodeScene::getTitle();
			ss << cpg::linefeed;
			ss << "[F] : " << step_mole::game_test::SpawnAndHitScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[SPACE] : " << step_mole::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
			);
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 0, 9, 61, 255 ) );
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
			_director->replaceScene( step_mole::animation::CallbackScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_mole::animation::ComponentScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_mole::collision::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( step_mole::collision::ComponentScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_mole::game_test::ObjectActionScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_S:
			_director->replaceScene( step_mole::game_test::SpawnScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_D:
			_director->replaceScene( step_mole::game_test::HittingNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_F:
			_director->replaceScene( step_mole::game_test::SpawnAndHitScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( step_mole::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
