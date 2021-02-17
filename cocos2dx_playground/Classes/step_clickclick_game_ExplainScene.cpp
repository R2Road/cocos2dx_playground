#include "step_clickclick_game_ExplainScene.h"

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

#include "cpg_StringTable.h"

#include "step_clickclick_game_PlayScene.h"
#include "step_clickclick_game_TitleScene.h"

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
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 6 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
				addChild( label, std::numeric_limits<int>::max() );
			}


			//
			// Subject
			//
			{
				auto label = Label::createWithTTF( "Chanllenge to Remove All Blocks", cpg::StringTable::GetFontPath(), 18 );
				label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.86f )
				);
				addChild( label );
			}

			//
			// Single Explain
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_clickclick_block_single.png" );
				sprite->setScale( _director->getContentScaleFactor() );
				sprite->setPosition( 
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.7f ) );
				addChild( sprite );

				auto label = Label::createWithTTF( ":   Decrease MySelf", cpg::StringTable::GetFontPath(), 14 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				label->setPosition( sprite->getPosition() + Vec2( sprite->getBoundingBox().size.width + 6.f, 0.f ) );
				addChild( label );
			}

			//
			// Together Explain
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_clickclick_block_together.png" );
				sprite->setScale( _director->getContentScaleFactor() );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.52f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( ":   3 X 3\n   Same Number Decrease\n   Different Number Increase", cpg::StringTable::GetFontPath(), 14 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				label->setPosition( sprite->getPosition() + Vec2( sprite->getBoundingBox().size.width + 6.f, 0.f ) );
				addChild( label );
			}

			//
			// Different Explain
			//
			{
				auto sprite = Sprite::createWithSpriteFrameName( "step_clickclick_block_different.png" );
				sprite->setScale( _director->getContentScaleFactor() );
				sprite->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.3f, visibleSize.height * 0.34f )
				);
				addChild( sprite );

				auto label = Label::createWithTTF( ":   3 X 3\n   Different Number Die\n   Same Number Increase", cpg::StringTable::GetFontPath(), 14 );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				label->setPosition( sprite->getPosition() + Vec2( sprite->getBoundingBox().size.width + 6.f, 0.f ) );
				addChild( label );
			}


			//
			// request input
			//
			{
				auto request_input_label = Label::createWithTTF( "PRESS SPACE BAR", cpg::StringTable::GetFontPath(), 14 );
				request_input_label->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.18f )
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

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ExplainScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void ExplainScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void ExplainScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_clickclick::game::TitleScene::create() );
				return;
			}

			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				_director->replaceScene( step_clickclick::game::PlayScene::create() );
				return;
			}
		}
	}
}
