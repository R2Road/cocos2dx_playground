#include "step_clickclick_game_ResultScene.h"

#include <new>
#include <sstream>
#include <iomanip>

#include "step_clickclick_game_TitleScene.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		ResultScene::ResultScene() : mKeyboardListener( nullptr ) {}

		Scene* ResultScene::create( const int clear_score )
		{
			auto ret = new ( std::nothrow ) ResultScene();
			if( !ret || !ret->init( clear_score ) )
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

		bool ResultScene::init( const int clear_score )
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
				ss << "[ESC] : Return to Title";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 8 );
				label->setColor( Color3B::BLACK );
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
				auto label = Label::createWithTTF( StringUtils::format( "Clear Time : %d", clear_score ), "fonts/arial.ttf", 32 );
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

			Node::onExit();
		}

		void ResultScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_clickclick::game::TitleScene::create() );
				return;
			}
		}
	}
}
