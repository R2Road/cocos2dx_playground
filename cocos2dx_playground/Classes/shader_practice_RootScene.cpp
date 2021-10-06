#include "shader_practice_RootScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "PlayGroundScene.h"
#include "shader_practice_BasicScene.h"
#include "shader_practice_GLUniformInfosScene.h"
#include "shader_practice_SimpleTestScene.h"
#include "shader_practice_CCTimeScene.h"
#include "shader_practice_CCRandom01Scene.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

USING_NS_CC;

namespace shader_practice
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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Playground";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[1] : " << shader_practice::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << "[2] : " << shader_practice::GLUniformInfosScene::getTitle();
			ss << cpg::linefeed;
			ss << "[3] : " << shader_practice::SimpleTestScene::getTitle();
			ss << cpg::linefeed;
			ss << "[4] : " << shader_practice::CCTimeScene::getTitle();
			ss << cpg::linefeed;
			ss << "[5] : " << shader_practice::CCRandom01Scene::getTitle();

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
			auto background_layer = LayerColor::create( Color4B( 51, 25, 73, 255 ) );
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
			_director->replaceScene( shader_practice::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( shader_practice::GLUniformInfosScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( shader_practice::SimpleTestScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( shader_practice::CCTimeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( shader_practice::CCRandom01Scene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
