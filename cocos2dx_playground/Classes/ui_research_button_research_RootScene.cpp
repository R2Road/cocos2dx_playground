#include "ui_research_button_research_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_StringTable.h"

#include "ui_research_button_research_OnMouseOverScene.h"
#include "ui_research_button_research_MouseRightClickScene.h"
#include "ui_research_button_research_EXButtonScene.h"
#include "ui_research_button_research_TeamFightManagerChampionSelectButtonScene.h"
#include "ui_research_button_research_TeamFightManagerChampionSelectButtonStepScene.h"
#include "ui_research_RootScene.h"

USING_NS_CC;

namespace ui_research
{
	namespace button_research
	{
		RootScene::RootScene() : mKeyboardListener( nullptr )
		{}

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
			const Vec2 visibleCenter(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			);

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[1] : " << ui_research::button_research::OnMouseOverScene::getTitle();
				ss << std::endl;
				ss << "[2] : " << ui_research::button_research::MouseRightClickScene::getTitle();
				ss << std::endl;
				ss << "[3] : " << ui_research::button_research::EXButtonScene::getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "=============================";
				ss << std::endl;
				ss << std::endl;
				ss << "[Q] : " << ui_research::button_research::TeamFightManagerChampionSelectButtonStepScene::getTitle();
				ss << std::endl;
				ss << "[W] : " << ui_research::button_research::TeamFightManagerChampionSelectButtonScene::getTitle();

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 11, Size::ZERO, TextHAlignment::LEFT );
				label->setPosition( visibleCenter );
				addChild( label, std::numeric_limits<int>::max() );
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
				_director->replaceScene( ui_research::RootScene::create() );
				return;

			case EventKeyboard::KeyCode::KEY_1:
				_director->replaceScene( button_research::OnMouseOverScene::create( helper::CreateSceneMover<button_research::RootScene>() ) );
				return;
			case EventKeyboard::KeyCode::KEY_2:
				_director->replaceScene( button_research::MouseRightClickScene::create( helper::CreateSceneMover<button_research::RootScene>() ) );
				return;
			case EventKeyboard::KeyCode::KEY_3:
				_director->replaceScene( button_research::EXButtonScene::create( helper::CreateSceneMover<button_research::RootScene>() ) );
				return;

			case EventKeyboard::KeyCode::KEY_Q:
				_director->replaceScene( button_research::TeamFightManagerChampionSelectButtonStepScene::create( helper::CreateSceneMover<button_research::RootScene>() ) );
				return;
			case EventKeyboard::KeyCode::KEY_W:
				_director->replaceScene( button_research::TeamFightManagerChampionSelectButtonScene::create( helper::CreateSceneMover<button_research::RootScene>() ) );
				return;
			}
		}
	}
}
