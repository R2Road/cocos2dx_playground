#include "step_pathfinder_game_ResultScene.h"

#include <new>
#include <sstream>
#include <iomanip>

#include "step_pathfinder_game_TitleScene.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace game
	{
		ResultScene::ResultScene() : mKeyboardListener( nullptr ) {}

		Scene* ResultScene::create( const float clear_time )
		{
			auto ret = new ( std::nothrow ) ResultScene();
			if( !ret || !ret->init( clear_time ) )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool ResultScene::init( const float clear_time )
		{
			if( !Scene::init() )
				return false;

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Return to Title";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B::WHITE );
				addChild( background_layer, 0 );
			}

			//
			// Game Clear
			//
			{
				auto label = Label::createWithTTF( "Game Clear", "fonts/arial.ttf", 32, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::BLACK );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.65f )
				) );
				addChild( label, 10 );
			}

			//
			// Result
			//
			{
				char buffer[100];
				sprintf( buffer, "Clear Time : %.2f", clear_time );

				auto label = Label::createWithTTF( buffer, "fonts/arial.ttf", 32, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::BLACK );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.35f )
				) );
				addChild( label, 10 );
			}

			return true;
		}

		void ResultScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ResultScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void ResultScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void ResultScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_pathfinder::game::TitleScene::create() );
		}

		void ResultScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( ResultScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( ResultScene::updateForExit ), 0.f );
				}
				return;
			}
		}
	}
}
