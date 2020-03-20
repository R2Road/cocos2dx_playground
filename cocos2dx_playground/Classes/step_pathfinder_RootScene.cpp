#include "step_pathfinder_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "PlayGroundScene.h"

#include "step_pathfinder_Json_LoadNSaveScene.h"
#include "Step01_TextField_BasicScene.h"

#include "Step01_Tool_MapToolScene.h"
#include "Step01_Game_PathFinder_TitleScene.h"

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
			ss << "[1] : " << step_pathfinder::json::LoadNSaveScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << step01::text_field::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[3] : " << step01::tool::MapToolScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] : " << step01::game::pathfinder::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 41, 0, 61, 255 ) );
			ret->addChild( background_layer, -1 );
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
			Director::getInstance()->replaceScene( step_pathfinder::json::LoadNSaveScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( step01::text_field::BasicScene::create() );
			break;


		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( step01::tool::MapToolScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( step01::game::pathfinder::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
