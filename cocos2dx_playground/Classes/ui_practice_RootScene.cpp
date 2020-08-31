#include "ui_practice_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

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
			ss << "[1] : " << ui_practice::MouseCursorScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[2] : " << ui_practice::LayoutVerticalScene::getTitle();
			ss << std::endl;
			ss << "[3] : " << ui_practice::LayoutHorizontalScene::getTitle();
			ss << std::endl;
			ss << "[4] : " << ui_practice::RelativeLayoutScene::getTitle();
			ss << std::endl;
			ss << "[5] : " << ui_practice::LayoutNScrollViewScene::getTitle();
			ss << std::endl;
			ss << "[6] : " << ui_practice::layout_test::ChangeSequenceScene::getTitle();
			ss << std::endl;
			ss << "[7] : " << ui_practice::layout_test::VariousItemSizeScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[8] : " << ui_practice::ScrollViewGeneratorScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] : " << ui_practice::UIAndCamera::getTitle();
			ss << std::endl;
			ss << "[W] : " << ui_practice::Minimap::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : " << ui_practice::SliderScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[S] : " << ui_practice::ToggleButtonScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[D] : " << ui_practice::RadioButtonScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 99, 1, 0, 255 ) );
			ret->addChild( background_layer, std::numeric_limits<int>::min() );
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
			Director::getInstance()->replaceScene( ui_practice::MouseCursorScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( ui_practice::LayoutVerticalScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( ui_practice::LayoutHorizontalScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( ui_practice::RelativeLayoutScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_5:
			Director::getInstance()->replaceScene( ui_practice::LayoutNScrollViewScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_6:
			Director::getInstance()->replaceScene( ui_practice::layout_test::ChangeSequenceScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_7:
			Director::getInstance()->replaceScene( ui_practice::layout_test::VariousItemSizeScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_8:
			Director::getInstance()->replaceScene( ui_practice::ScrollViewGeneratorScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			Director::getInstance()->replaceScene( ui_practice::UIAndCamera::create() );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			Director::getInstance()->replaceScene( ui_practice::Minimap::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			Director::getInstance()->replaceScene( ui_practice::SliderScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_S:
			Director::getInstance()->replaceScene( ui_practice::ToggleButtonScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;
		case EventKeyboard::KeyCode::KEY_D:
			Director::getInstance()->replaceScene( ui_practice::RadioButtonScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
