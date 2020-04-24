#include "step_mole_game_TitleScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_mole_RootScene.h"

USING_NS_CC;

namespace step_mole
{
	namespace game
	{
		TitleScene::TitleScene() : mKeyboardListener( nullptr ) {}

		Scene* TitleScene::create()
		{
			auto ret = new ( std::nothrow ) TitleScene();
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

			return ret;
		}

		bool TitleScene::init()
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
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 8 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}


			//
			// request input
			//
			{
				auto request_input_label = Label::createWithTTF( "PRESS SPACE BAR", "fonts/arial.ttf", 14 );
				request_input_label->setPosition(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.23f
				);
				addChild( request_input_label, 1 );

				auto fadeOutAction = FadeOut::create( 0.8f );
				auto fadeOutkDelay = DelayTime::create( 0.2f );
				auto fadeInAction = FadeIn::create( 0.6f );
				auto fadeInkDelay = DelayTime::create( 0.4f );
				auto blinkSequence = Sequence::create( fadeOutAction, fadeOutkDelay, fadeInAction, fadeInkDelay, nullptr );
				auto blinkrepeat = RepeatForever::create( blinkSequence );
				request_input_label->runAction( blinkrepeat );
			}

			return true;
		}

		void TitleScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TitleScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void TitleScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void TitleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_mole::RootScene::create() );
				return;
			}
		}
	}
}