#include "step_typetype_BasicScene.h"

#include <new>

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace step_typetype
{
	BasicScene::BasicScene() : mKeyboardListener( nullptr ) {}

	Scene* BasicScene::create()
	{
		auto ret = new ( std::nothrow ) BasicScene();
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

	bool BasicScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();

		//
		// Label
		//
		{
			const char* label_string = "This is Test Label ^_^\n\nESC Key : Return to Root";

			auto label = Label::createWithTTF( label_string, "fonts/NanumSquareR.ttf", 30 );
			label->setColor( Color3B::GREEN );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( label );
		}

		return true;
	}

	void BasicScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void BasicScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			_director->replaceScene( step_typetype::RootScene::create() );
			return;
		}
	}
}
