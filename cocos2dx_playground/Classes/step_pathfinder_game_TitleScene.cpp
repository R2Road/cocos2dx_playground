#include "step_pathfinder_game_TitleScene.h"

#include <new>
#include <sstream>

#include "step_pathfinder_RootScene.h"

#include "step_pathfinder_game_PlayScene.h"
#include "step_pathfinder_game_ResultScene.h"

#include "CPG_Setting.h"

USING_NS_CC;

namespace step_pathfinder
{
	namespace game
	{
		namespace pathfinder
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
					ss << std::endl;
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

			void TitleScene::onEnter()
			{
				Scene::onEnter();

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

			void TitleScene::updateForExit( float /*dt*/ )
			{
				Director::getInstance()->replaceScene( step_pathfinder::RootScene::create() );
			}

			void TitleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
			{
				if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
				{
					if( !isScheduled( schedule_selector( TitleScene::updateForExit ) ) )
					{
						scheduleOnce( schedule_selector( TitleScene::updateForExit ), 0.f );
					}
					return;
				}

				if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
				{
					Director::getInstance()->replaceScene( step_pathfinder::game::pathfinder::PlayScene::create() );
					return;
				}

				if( EventKeyboard::KeyCode::KEY_F1 == keycode )
				{
					Director::getInstance()->replaceScene( step_pathfinder::game::pathfinder::ResultScene::create( 54321.12345f ) );
					return;
				}
			}
		} // namespace pathfinder
	}
}
