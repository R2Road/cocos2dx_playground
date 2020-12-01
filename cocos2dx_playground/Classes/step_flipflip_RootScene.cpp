#include "step_flipflip_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "PlayGroundScene.h"

#include "step_flipflip_texture_AliasScene.h"
#include "step_flipflip_texture_PListScene.h"
#include "step_flipflip_texture_PList4ThisProjectScene.h"
#include "step_flipflip_scale_BasicScene.h"
#include "step_flipflip_opacity_BasicScene.h"
#include "step_flipflip_action_BasicScene.h"

#include "step_flipflip_game_test_CardViewScene.h"

USING_NS_CC;

namespace step_flipflip
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
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Playground";
			ss << std::endl;
			ss << std::endl;
			ss << "[1] " << step_flipflip::texture::AliasScene::getTitle();
			ss << std::endl;
			ss << "[2] " << step_flipflip::texture::PListScene::getTitle();
			ss << std::endl;
			ss << "[3] " << step_flipflip::texture::PList4ThisProjectScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] " << step_flipflip::scale::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[5] " << step_flipflip::opacity::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[6] " << step_flipflip::action::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] " << step_flipflip::game_test::CardViewScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 36, 120, 122, 255 ) );
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
			_director->replaceScene( step_flipflip::texture::AliasScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_flipflip::texture::PListScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_flipflip::texture::PList4ThisProjectScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( step_flipflip::scale::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( step_flipflip::opacity::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_6:
			_director->replaceScene( step_flipflip::action::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step_flipflip::game_test::CardViewScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
