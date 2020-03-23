#include "step_pathfinder_game_TitleScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_pathfinder_RootScene.h"

#include "step_pathfinder_game_PlayScene.h"
#include "step_pathfinder_game_ResultScene.h"

USING_NS_CC;

namespace step_pathfinder
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
				ss << std::endl;
				ss << "[F1] : Result Scene Test";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}


			//
			// Title
			//
			{
				auto title = Sprite::create( "textures/step_pathfinder/step_pathfinder_title.png" );
				title->setScaleX( visibleSize.width / title->getContentSize().width );
				title->setScaleY( visibleSize.height / title->getContentSize().height );
				title->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.5f
				) );
				addChild( title, 0 );
			}


			//
			// request input
			//
			{
				auto request_input_label = Label::createWithTTF( "PRESS SPACE BAR", "fonts/arial.ttf", 12 );
				request_input_label->setPosition(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.3f
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
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
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
				Director::getInstance()->replaceScene( step_pathfinder::RootScene::create() );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				Director::getInstance()->replaceScene( step_pathfinder::game::PlayScene::create() );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_F1 == keycode )
			{
				Director::getInstance()->replaceScene( step_pathfinder::game::ResultScene::create( 54321.12345f ) );
				return;
			}
		}
	}
}
