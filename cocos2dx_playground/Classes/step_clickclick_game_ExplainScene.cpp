#include "step_clickclick_game_ExplainScene.h"

#include <new>
#include <sstream>

#include "2d/CCActionInterval.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_clickclick_game_PlayScene.h"
#include "step_clickclick_game_TitleScene.h"

#include "CPG_Setting.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		ExplainScene::ExplainScene() : mKeyboardListener( nullptr ) {}

		Scene* ExplainScene::create()
		{
			auto ret = new ( std::nothrow ) ExplainScene();
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

		bool ExplainScene::init()
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
				ss << std::endl;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 8 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}


			//
			// Subject
			//
			{
				auto label = Label::createWithTTF( "Chanllenge to Remove All Blocks", "fonts/arial.ttf", 18 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.86f
				) );
				addChild( label, 9999 );
			}

			//
			// Single Explain
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_clickclick_pannel_single.png" );
				sprite->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				sprite->setScale( 2.f );
				sprite->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.3f
					, visibleOrigin.y + visibleSize.height * 0.7f
				) );
				addChild( sprite, 0 );

				auto label = Label::createWithTTF( ":   Decrease MySelf", "fonts/arial.ttf", 14 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				label->setPosition( sprite->getPosition() + Vec2( sprite->getBoundingBox().size.width + 6.f, 0.f ) );
				addChild( label, 9999 );
			}

			//
			// Together Explain
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_clickclick_pannel_together.png" );
				sprite->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				sprite->setScale( 2.f );
				sprite->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.3f
					, visibleOrigin.y + visibleSize.height * 0.52f
				) );
				addChild( sprite, 0 );

				auto label = Label::createWithTTF( ":   Same Number Decrease\n   Different Number Increase", "fonts/arial.ttf", 14 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				label->setPosition( sprite->getPosition() + Vec2( sprite->getBoundingBox().size.width + 6.f, 0.f ) );
				addChild( label, 9999 );
			}

			//
			// Different Explain
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_clickclick_pannel_different.png" );
				sprite->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				sprite->setScale( 2.f );
				sprite->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width * 0.3f
					, visibleOrigin.y + visibleSize.height * 0.34f
				) );
				addChild( sprite, 0 );

				auto label = Label::createWithTTF( ":   Different Number Die\n   Same Number Increase", "fonts/arial.ttf", 14 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				label->setPosition( sprite->getPosition() + Vec2( sprite->getBoundingBox().size.width + 6.f, 0.f ) );
				addChild( label, 9999 );
			}


			//
			// request input
			//
			{
				auto request_input_label = Label::createWithTTF( "PRESS SPACE BAR", "fonts/arial.ttf", 14 );
				request_input_label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				request_input_label->setPosition(
					visibleOrigin.x + visibleSize.width * 0.5f
					, visibleOrigin.y + visibleSize.height * 0.18f
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

		void ExplainScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ExplainScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void ExplainScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void ExplainScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step_clickclick::game::TitleScene::create() );
		}

		void ExplainScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( ExplainScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( ExplainScene::updateForExit ), 0.f );
				}
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				Director::getInstance()->replaceScene( step_clickclick::game::PlayScene::create() );
				return;
			}
		}
	}
}
