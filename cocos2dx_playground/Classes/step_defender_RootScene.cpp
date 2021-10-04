#include "step_defender_RootScene.h"

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

#include "step_defender_parallax_node_BasicScene.h"
#include "step_defender_parallax_node_RatioCheckScene.h"

#include "step_defender_chipmunk_BasicScene.h"
#include "step_defender_chipmunk_GravityScene.h"
#include "step_defender_chipmunk_MoveScene.h"
#include "step_defender_chipmunk_ContactScene.h"

#include "ui_research_layout_research_ToolBarNodeScene.h"

#include "step_defender_tool_test_TileSheetNodeScene.h"
#include "step_defender_game_test_TileMapNodeScene.h"

#include "step_defender_tool_ParallaxLayerEditorScene.h"

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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Playground";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[1] : " << step_defender::parallax_node::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << "[2] : " << step_defender::parallax_node::RatioCheckScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[3] : " << step_defender::chipmunk::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << "[4] : " << step_defender::chipmunk::GravityScene::getTitle();
			ss << cpg::linefeed;
			ss << "[5] : " << step_defender::chipmunk::MoveScene::getTitle();
			ss << cpg::linefeed;
			ss << "[6] : " << step_defender::chipmunk::ContactScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[7] : " << cpgui::research_layout::ToolBarNodeScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Q] : " << step_defender::tool_test::TileSheetNodeScene::getTitle();
			ss << cpg::linefeed;
			ss << "[W] : " << step_defender::game_test::TileMapNodeScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[A] : " << step_defender::tool::ParallaxLayerEditorScene::getTitle();

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
			_director->replaceScene( step_defender::chipmunk::GravityScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( step_defender::chipmunk::MoveScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_6:
			_director->replaceScene( step_defender::chipmunk::ContactScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_7:
			_director->replaceScene( cpgui::research_layout::ToolBarNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step_defender::tool_test::TileSheetNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( step_defender::game_test::TileMapNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;


		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_defender::tool::ParallaxLayerEditorScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
