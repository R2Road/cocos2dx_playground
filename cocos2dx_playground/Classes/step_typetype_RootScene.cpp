#include "step_typetype_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "cocos2d.h"

#include "PlayGroundScene.h"

#include "step_typetype_label_ttf_AlignScene.h"
#include "step_typetype_label_ttf_ETCScene.h"

#include "step_typetype_sprite_BasicScene.h"
#include "step_typetype_node_AnchorPointScene.h"

#include "step_typetype_input_KeyCodeViewScene.h"

#include "step_typetype_sound_BasicScene.h"

#include "step_typetype_ScheduleScene.h"

#include "step_typetype_game_test_LetterViewNodeScene.h"
#include "step_typetype_game_test_StageViewNodeScene.h"
#include "step_typetype_game_test_IndicatorViewNodeScene.h"
#include "step_typetype_game_test_GameProcessScene.h"
#include "step_typetype_game_TitleScene.h"

USING_NS_CC;

namespace step_typetype
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
			ss << "[1] " << step_typetype::label_ttf::AlignScene::getTitle();
			ss << std::endl;
			ss << "[2] " << step_typetype::label_ttf::ETCScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[3] " << step_typetype::input::KeyCodeViewScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] " << step_typetype::sprite::BasicScene::getTitle();
			ss << std::endl;
			ss << "[5] " << step_typetype::node::AnchorPointScene::getTitle();
			ss << std::endl;
			ss << "[6] " << step_typetype::sound::BasicScene::getTitle();
			ss << std::endl;
			ss << "[7] " << step_typetype::ScheduleScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] " << step_typetype::game_test::LetterViewNodeScene::getTitle();
			ss << std::endl;
			ss << "[W] " << step_typetype::game_test::StageViewNodeScene::getTitle();
			ss << std::endl;
			ss << "[E] " << step_typetype::game_test::IndicatorViewNodeScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[A] " << step_typetype::game_test::GameProcessScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[SPACE] " << step_typetype::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( label );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 99, 1, 0, 255 ) );
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
			_director->replaceScene( step_typetype::label_ttf::AlignScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_typetype::label_ttf::ETCScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_typetype::input::KeyCodeViewScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( step_typetype::sprite::BasicScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( step_typetype::node::AnchorPointScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_6:
			_director->replaceScene( step_typetype::sound::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_7:
			_director->replaceScene( step_typetype::ScheduleScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step_typetype::game_test::LetterViewNodeScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( step_typetype::game_test::StageViewNodeScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_E:
			_director->replaceScene( step_typetype::game_test::IndicatorViewNodeScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_typetype::game_test::GameProcessScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( step_typetype::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
