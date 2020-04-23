#include "ui_practice_RootScene.h"

#include <new>
#include <sstream>

#include "PlayGroundScene.h"

#include "ui_practice_DirectionalLayoutScene.h"
#include "ui_practice_LayoutNScrollViewScene.h"
#include "ui_practice_UIAndCamera.h"
#include "ui_practice_Minimap.h"
#include "ui_practice_SliderScene.h"

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
			ss << "[1] : " << ui_practice::DirectionalLayoutScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << ui_practice::LayoutNScrollViewScene::getTitle();
			ss << std::endl;
			ss << "[3] : " << ui_practice::UIAndCamera::getTitle();
			ss << std::endl;
			ss << "[4] : " << ui_practice::Minimap::getTitle();
			ss << std::endl;
			ss << "[5] : " << ui_practice::SliderScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
			Director::getInstance()->replaceScene( ui_practice::DirectionalLayoutScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( ui_practice::LayoutNScrollViewScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( ui_practice::UIAndCamera::create() );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( ui_practice::Minimap::create() );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			Director::getInstance()->replaceScene( ui_practice::SliderScene::create( helper::CreateSceneMover<RootScene>() ) );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
