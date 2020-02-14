#include "Step01_RootScene.h"

#include <new>
#include <sstream>

#include "PlayGroundScene.h"

#include "Step01_Input_RootScene.h"
#include "Step01_Animation_RootScene.h"
#include "Step01_Json_LoadNSaveScene.h"
#include "Step01_Button_BasicScene.h"
#include "Step01_TextField_BasicScene.h"
#include "Step01_Sound_Basic.h"

#include "Step01_Tool_MapToolScene.h"
#include "Step01_Game_PathFinder_TitleScene.h"

USING_NS_CC;

namespace step01
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
			ss << "+ Step 01 : Input And Animation";
			ss << "\n";
			ss << "\n";
			ss << "[ESC] : Return to Playground";
			ss << "\n";
			ss << "\n";
			ss << "[1] : Input";
			ss << "\n";
			ss << "[2] : Animation";
			ss << "\n";
			ss << "[3] : " << json::LoadNSaveScene::getTitle();
			ss << "\n";
			ss << "[4] : " << step01::button::BasicScene::getTitle();
			ss << "\n";
			ss << "[5] : " << step01::text_field::BasicScene::getTitle();
			ss << "\n";
			ss << "[6] : " << step01::sound::BasicScene::getTitle();
			ss << "\n";
			ss << "\n";
			ss << "[7] : " << tool::MapToolScene::getTitle();
			ss << "\n";
			ss << "[8] : " << game::pathfinder::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label, 1 );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 41, 0, 61, 255 ) );
			ret->addChild( background_layer, 0 );
		}

		return ret;
	}

	void RootScene::onEnter()
	{
		Scene::onEnter();

		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
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
			Director::getInstance()->replaceScene( input::RootScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( animation::RootScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( json::LoadNSaveScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( step01::button::BasicScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			Director::getInstance()->replaceScene( step01::text_field::BasicScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_6:
			Director::getInstance()->replaceScene( step01::sound::BasicScene::create() );
			break;


		case EventKeyboard::KeyCode::KEY_7:
			Director::getInstance()->replaceScene( tool::MapToolScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_8:
			Director::getInstance()->replaceScene( game::pathfinder::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
