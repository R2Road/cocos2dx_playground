#include "input_practice_RootScene.h"

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

#include "Research_Input_KeyAllowScene.h"
#include "Research_Input_AllowedKeysTestScene.h"

#include "Research_Input_KeyConfigScene.h"
#include "Research_Input_ConfigedKeysTestScene.h"

#include "input_practice_GamePadTestScene.h"

USING_NS_CC;

namespace input_practice
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
			ss << "[1] : " << input_practice::GamePadTestScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[2] : Key Allow";
			ss << std::endl;
			ss << "[3] : Allowed Keys Test";
			ss << std::endl;
			ss << std::endl;
			ss << "[4] : Key Config";
			ss << std::endl;
			ss << "[5] : Configed Keys Test";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 44, 112, 87, 255 ) );
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
			_director->replaceScene( input_practice::GamePadTestScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( research::input::KeyAllowScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( research::input::AllowedKeysTestScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( input_practice::KeyConfigScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( research::input::ConfigedKeysTestScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace input_practice
