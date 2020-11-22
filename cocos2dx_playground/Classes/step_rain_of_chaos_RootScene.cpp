#include "step_rain_of_chaos_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

#include "PlayGroundScene.h"

#include "step_rain_of_chaos_input_KeyCodeCollectScene.h"
#include "step_rain_of_chaos_batch_node_BasicScene.h"
#include "step_rain_of_chaos_batch_node_QuadScene.h"
#include "step_rain_of_chaos_button_DragScene.h"
#include "step_rain_of_chaos_draw_node_CircleScene.h"
#include "ui_practice_LayoutVerticalScene.h"

#include "step_rain_of_chaos_game_test_ActorMoveScene.h"
#include "step_rain_of_chaos_game_test_BackgroundNodeScene.h"
#include "step_rain_of_chaos_game_test_BulletLifeComponentScene.h"
#include "step_rain_of_chaos_game_test_EnemyProcessorScene.h"
#include "step_rain_of_chaos_game_test_StageNodeScene.h"
#include "step_rain_of_chaos_game_test_SpawnProcessorScene.h"

#include "step_rain_of_chaos_game_TitleScene.h"

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
			ss << "[1] : " << step_rain_of_chaos::input::KeyCodeCollectScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[2] : " << step_rain_of_chaos::batch_node::BasicScene::getTitle();
			ss << std::endl;
			ss << "[3] : " << step_rain_of_chaos::batch_node::QuadScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] : " << step_rain_of_chaos::button::DragScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[5] : " << step_rain_of_chaos::draw_node::CircleScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[6] : " << ui_practice::LayoutVerticalScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] : " << step_rain_of_chaos::game_test::ActorMoveScene::getTitle();
			ss << std::endl;
			ss << "[W] : " << step_rain_of_chaos::game_test::BackgroundNodeScene::getTitle();
			ss << std::endl;
			ss << "[E] : " << step_rain_of_chaos::game_test::BulletLifeComponentScene::getTitle();
			ss << std::endl;
			ss << "[R] : " << step_rain_of_chaos::game_test::StageNodeScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : " << step_rain_of_chaos::game_test::SpawnProcessorScene::getTitle();
			ss << std::endl;
			ss << "[S] : " << step_rain_of_chaos::game_test::EnemyProcessorScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[SPACE] : " << step_rain_of_chaos::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 140, 54, 32, 255 ) );
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
			_director->replaceScene( step_rain_of_chaos::input::KeyCodeCollectScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_rain_of_chaos::batch_node::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_rain_of_chaos::batch_node::QuadScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( step_rain_of_chaos::button::DragScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( step_rain_of_chaos::draw_node::CircleScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_6:
			_director->replaceScene( ui_practice::LayoutVerticalScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;


		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step_rain_of_chaos::game_test::ActorMoveScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( step_rain_of_chaos::game_test::BackgroundNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_E:
			_director->replaceScene( step_rain_of_chaos::game_test::BulletLifeComponentScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_R:
			_director->replaceScene( step_rain_of_chaos::game_test::StageNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_rain_of_chaos::game_test::SpawnProcessorScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_S:
			_director->replaceScene( step_rain_of_chaos::game_test::EnemyProcessorScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;


		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( step_rain_of_chaos::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
