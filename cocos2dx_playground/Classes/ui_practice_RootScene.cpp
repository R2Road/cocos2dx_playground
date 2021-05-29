#include "ui_practice_RootScene.h"

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

#include "ui_practice_MouseCursorScene.h"
#include "ui_practice_LayoutVerticalScene.h"
#include "ui_practice_LayoutHorizontalScene.h"
#include "ui_practice_RelativeLayoutScene.h"
#include "ui_practice_LayoutNScrollViewScene.h"
#include "ui_practice_layout_test_ChangeSequenceScene.h"
#include "ui_practice_layout_test_VariousItemSizeScene.h"

#include "ui_practice_ScrollViewGeneratorScene.h"

#include "ui_practice_UIAndCamera.h"
#include "ui_practice_Minimap.h"

#include "ui_practice_SliderScene.h"
#include "ui_practice_ToggleButtonScene.h"
#include "ui_practice_RadioButtonScene.h"

USING_NS_CC;

namespace ui_practice
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
			ss << "[1] : " << ui_practice::MouseCursorScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[2] : " << ui_practice::LayoutVerticalScene::getTitle();
			ss << cpg::linefeed;
			ss << "[3] : " << ui_practice::LayoutHorizontalScene::getTitle();
			ss << cpg::linefeed;
			ss << "[4] : " << ui_practice::RelativeLayoutScene::getTitle();
			ss << cpg::linefeed;
			ss << "[5] : " << ui_practice::LayoutNScrollViewScene::getTitle();
			ss << cpg::linefeed;
			ss << "[6] : " << ui_practice::layout_test::ChangeSequenceScene::getTitle();
			ss << cpg::linefeed;
			ss << "[7] : " << ui_practice::layout_test::VariousItemSizeScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[8] : " << ui_practice::ScrollViewGeneratorScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Q] : " << ui_practice::UIAndCamera::getTitle();
			ss << cpg::linefeed;
			ss << "[W] : " << ui_practice::Minimap::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[A] : " << ui_practice::SliderScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[S] : " << ui_practice::ToggleButtonScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[D] : " << ui_practice::RadioButtonScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
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
			_director->replaceScene( ui_practice::MouseCursorScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( ui_practice::LayoutVerticalScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( ui_practice::LayoutHorizontalScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( ui_practice::RelativeLayoutScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( ui_practice::LayoutNScrollViewScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_6:
			_director->replaceScene( ui_practice::layout_test::ChangeSequenceScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_7:
			_director->replaceScene( ui_practice::layout_test::VariousItemSizeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_8:
			_director->replaceScene( ui_practice::ScrollViewGeneratorScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( ui_practice::UIAndCamera::create() );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( ui_practice::Minimap::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( ui_practice::SliderScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_S:
			_director->replaceScene( ui_practice::ToggleButtonScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_D:
			_director->replaceScene( ui_practice::RadioButtonScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
