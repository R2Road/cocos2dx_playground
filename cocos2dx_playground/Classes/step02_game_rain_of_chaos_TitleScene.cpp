#include "step02_game_rain_of_chaos_TitleScene.h"

#include <new>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "step_rain_of_chaos_RootScene.h"

USING_NS_CC;

namespace step02
{
	namespace game
	{
		namespace rain_of_chaos
		{
			TitleScene::TitleScene() : mKeyboardListener( nullptr )
			{}

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
					ss << "+ " << getTitle();
					ss << std::endl;
					ss << std::endl;
					ss << "[ESC] : Return to Root";

					auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
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
					auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
					addChild( background_layer, 0 );
				}

				return true;
			}

			void TitleScene::onEnter()
			{
				Scene::onEnter();

				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( TitleScene::onKeyPressed, this );
				getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
			}
			void TitleScene::onExit()
			{
				assert( mKeyboardListener );
				getEventDispatcher()->removeEventListener( mKeyboardListener );
				mKeyboardListener = nullptr;

				Scene::onExit();
			}

			void TitleScene::updateForExit( float /*dt*/ )
			{
				Director::getInstance()->replaceScene( step_rain_of_chaos::RootScene::create() );
			}

			void TitleScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
			{
				switch( keycode )
				{
				case EventKeyboard::KeyCode::KEY_ESCAPE:
					if( !isScheduled( schedule_selector( TitleScene::updateForExit ) ) )
					{
						scheduleOnce( schedule_selector( TitleScene::updateForExit ), 0.f );
					}
					break;

				default:
					CCLOG( "Key Code : %d", keycode );
				}
			}
		}
	}
}
