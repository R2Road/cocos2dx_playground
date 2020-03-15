#include "step_typetype_RootScene.h"

#include <new>
#include <sstream>

#include "PlayGroundScene.h"

#include "step_typetype_sprite_TextureTypeScene.h"
#include "step_typetype_sprite_AnchorPointScene.h"

#include "step_typetype_sound_Basic.h"

#include "step_typetype_input_KeyCodeViewScene.h"

#include "step_typetype_game_StageTestScene.h"
#include "step_typetype_game_TitleScene.h"

USING_NS_CC;

namespace step_typetype
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
			ss << "[1] " << step_typetype::sprite::TextureTypeScene::getTitle();
			ss << std::endl;
			ss << "[2] " << step_typetype::sprite::AnchorPointScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[3] " << step_typetype::input::KeyCodeViewScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] " << step_typetype::sound::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[5] " << step_typetype::game::StageTestScene::getTitle();
			ss << std::endl;
			ss << "[6] " << step_typetype::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 99, 1, 0, 255 ) );
			ret->addChild( background_layer, -1 );
		}

		return ret;
	}

	void RootScene::onEnter()
	{
		Scene::onEnter();

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
			Director::getInstance()->replaceScene( step_typetype::sprite::TextureTypeScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( step_typetype::sprite::AnchorPointScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( step_typetype::input::KeyCodeViewScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( step_typetype::sound::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_5:
			Director::getInstance()->replaceScene( step_typetype::game::StageTestScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_6:
			Director::getInstance()->replaceScene( step_typetype::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01