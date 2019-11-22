#include "Research_KeyboardInputScene.h"

#include <sstream>

USING_NS_CC;

namespace Research
{
	KeyboardInputScene::KeyboardInputScene() : listener( nullptr ) {}

	Scene* KeyboardInputScene::create()
	{
		auto ret = new ( std::nothrow ) KeyboardInputScene();
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
		}


		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		std::stringstream ss;
		ss << "+ Keyboard Input Scene";
		ss << "\n";
		ss << "\n";
		ss << "[ESC] : Exit";

		auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
		label->setAnchorPoint( Vec2( 0.f, 1.f ) );
		label->setPosition( Vec2(
			origin.x
			, origin.y + visibleSize.height
		) );
		ret->addChild( label, 1 );


		ret->autorelease();

		return ret;
	}

	void KeyboardInputScene::onEnter()
	{
		Scene::onEnter();

		listener = EventListenerKeyboard::create();
		listener->onKeyPressed = CC_CALLBACK_2( KeyboardInputScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithFixedPriority( listener, 1 );
	}
	void KeyboardInputScene::onExit()
	{
		if( listener )
			getEventDispatcher()->removeEventListener( listener );
		Scene::onExit();
	}


	void KeyboardInputScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
		{
			cocos2d::Director::getInstance()->end();
		}
		break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
