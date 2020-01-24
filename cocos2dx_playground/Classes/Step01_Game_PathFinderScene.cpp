#include "Step01_Game_PathFinderScene.h"

#include <new>
#include <sstream>

#include "Step01_RootScene.h"

#include "Step01_Game_PathFinder_PlayScene.h"

#include "CPG_Setting.h"

USING_NS_CC;

namespace step01
{
	namespace game
	{
		PathFinderScene::PathFinderScene() : mKeyboardListener( nullptr ) {}

		Scene* PathFinderScene::create()
		{
			auto ret = new ( std::nothrow ) PathFinderScene();
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

		bool PathFinderScene::init()
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
				ss << "[ESC] : Return to Step 01 Root";

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
			// Title
			//
			{
				auto title = Sprite::create( "textures/step01_game_pathfinder/title.png" );
				title->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
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
				auto request_input_label = Label::createWithTTF( "PRESS SPACE BAR", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
				request_input_label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
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

		void PathFinderScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PathFinderScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void PathFinderScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void PathFinderScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( step01::RootScene::create() );
		}

		void PathFinderScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				if( !isScheduled( schedule_selector( PathFinderScene::updateForExit ) ) )
					scheduleOnce( schedule_selector( PathFinderScene::updateForExit ), 0.f );
				break;

			case EventKeyboard::KeyCode::KEY_SPACE:
				Director::getInstance()->replaceScene( step01::game::pathfinder::PlayScene::create() );
				break;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
