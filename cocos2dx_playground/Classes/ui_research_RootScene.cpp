#include "ui_research_RootScene.h"

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

#include "ui_research_button_research_RootScene.h"

#include "ui_research_type_effect_BasicScene.h"
#include "ui_research_type_effect_MultiByteScene.h"
#include "ui_research_type_effect_ColorScene.h"
#include "ui_research_type_effect_ShakeScene.h"

#include "ui_research_text_field_BMFontScene.h"

#include "cpgui_research_layout_ToolBarNodeScene.h"

USING_NS_CC;

namespace ui_research
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
			ss << "[1] : " << ui_research::button_research::RootScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[Q] : " << ui_research::type_effect::BasicScene::getTitle();
			ss << std::endl;
			ss << "[W] : " << ui_research::type_effect::MultiByteScene::getTitle();
			ss << std::endl;
			ss << "[E] : " << ui_research::type_effect::ColorScene::getTitle();
			ss << std::endl;
			ss << "[R] : " << ui_research::type_effect::ShakeScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[T] : " << ui_research::text_field::BMFontScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[A] : " << cpgui::research_layout::ToolBarNodeScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 11, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 30, 55, 92, 255 ) );
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
			return;

		case EventKeyboard::KeyCode::KEY_1:
			_director->replaceScene( ui_research::button_research::RootScene::create() );
			return;

		case EventKeyboard::KeyCode::KEY_Q:
			_director->replaceScene( ui_research::type_effect::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_W:
			_director->replaceScene( ui_research::type_effect::MultiByteScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_E:
			_director->replaceScene( ui_research::type_effect::ColorScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_R:
			_director->replaceScene( ui_research::type_effect::ShakeScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		case EventKeyboard::KeyCode::KEY_T:
			_director->replaceScene( ui_research::text_field::BMFontScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( cpgui::research_layout::ToolBarNodeScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
