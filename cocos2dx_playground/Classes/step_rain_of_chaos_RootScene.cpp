#include "step_rain_of_chaos_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "PlayGroundScene.h"

#include "step_rain_of_chaos_animation_ListScene.h"
#include "step_rain_of_chaos_animation_CallbackScene.h"
#include "step_rain_of_chaos_animation_ComponentScene.h"

#include "step02_input_KeyCodeCollectScene.h"

#include "step_rain_of_chaos_button_DragScene.h"

#include "step02_fsm1test_BasicScene.h"
#include "step02_fsm1test_AnimationControlScene.h"

#include "step02_collision_BasicScene.h"
#include "step02_collision_CollectionScene.h"
#include "step02_game_rain_of_chaos_TitleScene.h"

USING_NS_CC;

namespace step_rain_of_chaos
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
			ss << "[1] : " << step02::input::KeyCodeCollectScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << step02::collision::BasicScene::getTitle();
			ss << std::endl;
			ss << "[3] : " << step02::collision::CollectionScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] : " << step_rain_of_chaos::animation::ListScene::getTitle();
			ss << std::endl;
			ss << "[5] : " << step_rain_of_chaos::animation::CallbackScene::getTitle();
			ss << std::endl;
			ss << "[6] : " << step_rain_of_chaos::animation::ComponentScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[7] : " << step02::fsm1test::BasicScene::getTitle();
			ss << std::endl;
			ss << "[8] : " << step02::fsm1test::AnimationControlScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : " << step02::game::rain_of_chaos::TitleScene::getTitle();
			ss << std::endl;
			ss << "[S] : " << step_rain_of_chaos::button::DragScene::getTitle();

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
			Director::getInstance()->replaceScene( step02::input::KeyCodeCollectScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( step02::collision::BasicScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_3:
			Director::getInstance()->replaceScene( step02::collision::CollectionScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			Director::getInstance()->replaceScene( step_rain_of_chaos::animation::ListScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_5:
			Director::getInstance()->replaceScene( step_rain_of_chaos::animation::CallbackScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_6:
			Director::getInstance()->replaceScene( step_rain_of_chaos::animation::ComponentScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_7:
			Director::getInstance()->replaceScene( step02::fsm1test::BasicScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_8:
			Director::getInstance()->replaceScene( step02::fsm1test::AnimationControlScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			Director::getInstance()->replaceScene( step02::game::rain_of_chaos::TitleScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_S:
			Director::getInstance()->replaceScene( step_rain_of_chaos::button::DragScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
