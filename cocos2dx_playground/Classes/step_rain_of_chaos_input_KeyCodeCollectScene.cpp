#include "step_rain_of_chaos_input_KeyCodeCollectScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"

#include "cpg_StringTable.h"
#include "CPG_Input_KeyCodeNames.h"
#include "step_rain_of_chaos_RootScene.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace input
	{
		KeyCodeCollectScene::KeyCodeCollectScene() : mKeyboardListener( nullptr ), mKeyCodeCollector(), mKeyStrings( 100, 0 ), mKeyViewer( nullptr )
		{}

		Scene* KeyCodeCollectScene::create()
		{
			auto ret = new ( std::nothrow ) KeyCodeCollectScene();
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

		bool KeyCodeCollectScene::init()
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
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[Keyboard] : Do Type";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 130, 49, 29, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// key viewer
			//
			mKeyViewer = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::CENTER );
			mKeyViewer->setColor( Color3B::GREEN );
			mKeyViewer->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( mKeyViewer );

			return true;
		}

		void KeyCodeCollectScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( KeyCodeCollectScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyCodeCollectScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void KeyCodeCollectScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void KeyCodeCollectScene::updateForExit( float /*dt*/ )
		{
			_director->replaceScene( step_rain_of_chaos::RootScene::create() );
		}

		void KeyCodeCollectScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( KeyCodeCollectScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( KeyCodeCollectScene::updateForExit ), 0.f );
				}
			}
			else
			{
				mKeyCodeCollector.onKeyPressed( keycode );
				updateKeyViewer();
			}
		}
		void KeyCodeCollectScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			mKeyCodeCollector.onKeyReleased( keycode );
			updateKeyViewer();
		}

		void KeyCodeCollectScene::updateKeyViewer()
		{
			mKeyStrings.clear();
			for( auto cur = step_rain_of_chaos::input::KeyCodeContainerFirst; cur < step_rain_of_chaos::input::KeyCodeContainerSize; ++cur )
			{
				if( mKeyCodeCollector.isActiveKey( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) ) )
				{
					mKeyStrings += cpg_input::KeyCodeNames::Get( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) );
					mKeyStrings += "\n";
				}
			}
			mKeyViewer->setString( mKeyStrings );
		}
	}
}
