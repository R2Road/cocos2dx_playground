#include "step_flipflip_RootScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "cpg_SStream.h"

#include "PlayGroundScene.h"

#include "step_flipflip_texture_AliasScene.h"
#include "step_flipflip_texture_PListScene.h"
#include "step_flipflip_texture_PList4ThisProjectScene.h"

#include "step_flipflip_scale_BasicScene.h"
#include "step_flipflip_opacity_BasicScene.h"
#include "step_flipflip_sprite_frame_BasicScene.h"

#include "step_flipflip_action_BasicScene.h"

#include "step_flipflip_game_test_CardViewScene.h"
#include "step_flipflip_game_test_CardSelectorNodeScene.h"
#include "step_flipflip_game_test_StageViewScene.h"
#include "step_flipflip_game_test_SelectAndFlipScene.h"
#include "step_flipflip_game_test_MessageViewNodeScene.h"

#include "step_flipflip_game_TitleScene.h"

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
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Playground";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[1] " << step_flipflip::texture::AliasScene::getTitle();
			ss << cpg::linefeed;
			ss << "[2] " << step_flipflip::texture::PListScene::getTitle();
			ss << cpg::linefeed;
			ss << "[3] " << step_flipflip::texture::PList4ThisProjectScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[4] " << step_flipflip::scale::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << "[5] " << step_flipflip::opacity::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << "[6] " << step_flipflip::sprite_frame::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << "[7] " << step_flipflip::action::BasicScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[Q] " << step_flipflip::game_test::CardViewScene::getTitle();
			ss << cpg::linefeed;
			ss << "[W] " << step_flipflip::game_test::CardSelectorNodeScene::getTitle();
			ss << cpg::linefeed;
			ss << "[E] " << step_flipflip::game_test::StageViewScene::getTitle();
			ss << cpg::linefeed;
			ss << "[R] " << step_flipflip::game_test::SelectAndFlipScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[A] " << step_flipflip::game_test::MessageViewNodeScene::getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "=============================";
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[SPACE] " << step_flipflip::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition(
				visibleOrigin
				+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.5f )
			);
			addChild( label );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 33, 112, 112, 255 ) );
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
			_director->replaceScene( step_flipflip::sprite_frame::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_7:
			_director->replaceScene( step_flipflip::action::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( step_flipflip::game_test::CardViewScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( step_flipflip::game_test::CardSelectorNodeScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_E:
			_director->replaceScene( step_flipflip::game_test::StageViewScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_R:
			_director->replaceScene( step_flipflip::game_test::SelectAndFlipScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_flipflip::game_test::MessageViewNodeScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( step_flipflip::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
