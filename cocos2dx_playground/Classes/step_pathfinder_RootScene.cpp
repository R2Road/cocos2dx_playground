#include "step_pathfinder_RootScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "PlayGroundScene.h"

#include "step_pathfinder_fileutil_PathScene.h"
#include "step_pathfinder_fileutil_GenerateNDeleteScene.h"

#include "step_pathfinder_json_LoadNSaveScene.h"
#include "step_pathfinder_textField_BasicScene.h"

#include "step_pathfinder_game_test_TerrainViewerScene.h"
#include "step_pathfinder_game_test_PlayerMoveScene.h"
#include "step_pathfinder_tool_TerrainToolScene.h"

#include "step_pathfinder_game_TitleScene.h"

USING_NS_CC;

namespace step_pathfinder
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

		const auto visibleSize = Director::getInstance()->getVisibleSize();
		const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
		const Vec2 visibleCenter(
			visibleOrigin.x + ( visibleSize.width * 0.5f )
			, visibleOrigin.y + ( visibleSize.height * 0.5f )
		);

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
			ss << "[1] : " << step_pathfinder::fileutil::PathScene::getTitle();
			ss << cpg::linefeed;
			ss << "[2] : " << step_pathfinder::fileutil::GenerateNDeleteScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[3] : " << step_pathfinder::json::LoadNSaveScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[4] : " << step_pathfinder::text_field::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Q] : " << step_pathfinder::game_test::TerrainViewerScene::getTitle();
			ss << cpg::linefeed;
			ss << "[W] : " << step_pathfinder::game_test::PlayerMoveScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[A] : " << step_pathfinder::tool::TerrainToolScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[SPACE] : " << step_pathfinder::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 11, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition( visibleCenter );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 41, 0, 61, 255 ) );
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
			_director->replaceScene( step_pathfinder::fileutil::PathScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_pathfinder::fileutil::GenerateNDeleteScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_pathfinder::json::LoadNSaveScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( step_pathfinder::text_field::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step_pathfinder::game_test::TerrainViewerScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( step_pathfinder::game_test::PlayerMoveScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_pathfinder::tool::TerrainToolScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( step_pathfinder::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
