#include "ui_research_gauge_research_RootScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"

#include "ui_research_RootScene.h"

USING_NS_CC;

namespace ui_research
{
	namespace gauge_research
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "=============================";
				ss << cpg::linefeed;
				ss << cpg::linefeed;

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


		void RootScene::onKeyPressed( EventKeyboard::KeyCode key_code, Event* /*key_event*/ )
		{
			switch( key_code )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				_director->replaceScene( ui_research::RootScene::create() );
				return;
			}
		}
	}
}
