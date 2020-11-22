#include "step_rain_of_chaos_game_ResultScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "cpg_StringTable.h"

#include "step_rain_of_chaos_game_TitleScene.h"

USING_NS_CC;

namespace step_rain_of_chaos
{
	namespace game
	{
		ResultScene::ResultScene() : mKeyboardListener( nullptr ) {}

		Scene* ResultScene::create( const int total_wave_count, const int clear_wave_count )
		{
			auto ret = new ( std::nothrow ) ResultScene();
			if( !ret || !ret->init( total_wave_count, clear_wave_count ) )
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

		bool ResultScene::init( const int total_wave_count, const int clear_wave_count )
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
				ss << "[ESC] : Return to Title";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::BLACK );
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
				auto background_layer = LayerColor::create( total_wave_count == clear_wave_count ? Color4B::WHITE : Color4B::GRAY );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Game Clear
			//
			{
				auto label = Label::createWithTTF(
					total_wave_count == clear_wave_count ? "Game Clear" : "You Died"
					, cpg::StringTable::GetFontPath()
					, 32
				);
				label->setColor( Color3B::BLACK );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.6f )
				) );
				addChild( label );
			}

			//
			// Result
			//
			{
				auto label = Label::createWithTTF( StringUtils::format( "Wave : %d / %d", clear_wave_count, total_wave_count ) , cpg::StringTable::GetFontPath(), 32 );
				label->setColor( Color3B::BLACK );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.4f )
				) );
				addChild( label );
			}

			return true;
		}

		void ResultScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ResultScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ResultScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void ResultScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( TitleScene::create() );
				return;
			}
		}
	}
}
