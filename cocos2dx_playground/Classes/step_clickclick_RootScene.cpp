#include "step_clickclick_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_StringTable.h"

#include "PlayGroundScene.h"

#include "step_clickclick_animation_PlayNStopScene.h"
#include "step_clickclick_animation_ListScene.h"

#include "step_clickclick_button_BasicScene.h"

#include "step_clickclick_game_test_EffectViewScene.h"
#include "step_clickclick_game_test_BlockViewNodeScene.h"
#include "step_clickclick_game_test_StageViewNodeScene.h"
#include "step_clickclick_game_test_StageScene.h"

#include "step_clickclick_game_TitleScene.h"

USING_NS_CC;

namespace step_clickclick
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
			ss << "[1] : " << step_clickclick::animation::PlayNStopScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << step_clickclick::animation::ListScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[3] : " << step_clickclick::button::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] : " << step_clickclick::game_test::EffectViewScene::getTitle();
			ss << std::endl;
			ss << "[W] : " << step_clickclick::game_test::BlockViewNodeScene::getTitle();
			ss << std::endl;
			ss << "[E] : " << step_clickclick::game_test::StageViewNodeScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : " << step_clickclick::game::StageTestScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[SPACE] : " << step_clickclick::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 0, 61, 33, 255 ) );
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
			_director->replaceScene( PlayGroundScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_1:
			_director->replaceScene( step_clickclick::animation::PlayNStopScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_clickclick::animation::ListScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_clickclick::button::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step_clickclick::game_test::EffectViewScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( step_clickclick::game_test::BlockViewNodeScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_E:
			_director->replaceScene( step_clickclick::game_test::StageViewNodeScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_clickclick::game::StageTestScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( step_clickclick::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
