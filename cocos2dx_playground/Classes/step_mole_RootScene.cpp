#include "step_mole_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "step_mole_animation_ListScene.h"
#include "step_mole_animation_CallbackScene.h"
#include "step_mole_animation_ComponentScene.h"

#include "PlayGroundScene.h"

USING_NS_CC;

namespace step_mole
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
			ss << "[1] : " << step_mole::animation::ListScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << step_mole::animation::CallbackScene::getTitle();
			ss << std::endl;
			ss << "[3] : " << step_mole::animation::ComponentScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 0, 9, 61, 255 ) );
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
			Director::getInstance()->replaceScene( step_mole::animation::ListScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( step_mole::animation::CallbackScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( step_mole::animation::ComponentScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
