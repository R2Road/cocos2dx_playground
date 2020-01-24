#include "Step01_Game_PathFinder_PlayScene.h"

#include <new>
#include <sstream>

#include "Step01_Game_PathFinder_TitleScene.h"

USING_NS_CC;

namespace step01
{
	namespace game
	{
		namespace pathfinder
		{
			PlayScene::PlayScene() : mKeyboardListener( nullptr ) {}

			Scene* PlayScene::create()
			{
				auto ret = new ( std::nothrow ) PlayScene();
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

			bool PlayScene::init()
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

				return true;
			}

			void PlayScene::onEnter()
			{
				Scene::onEnter();

				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( PlayScene::onKeyPressed, this );
				getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
			}
			void PlayScene::onExit()
			{
				assert( mKeyboardListener );
				getEventDispatcher()->removeEventListener( mKeyboardListener );
				mKeyboardListener = nullptr;

				Node::onExit();
			}

			void PlayScene::updateForExit( float /*dt*/ )
			{
				Director::getInstance()->replaceScene( game::pathfinder::TitleScene::create() );
			}

			void PlayScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
			{
				switch( keycode )
				{
				case EventKeyboard::KeyCode::KEY_ESCAPE:
					if( !isScheduled( schedule_selector( PlayScene::updateForExit ) ) )
						scheduleOnce( schedule_selector( PlayScene::updateForExit ), 0.f );
					break;

				default:
					CCLOG( "Key Code : %d", keycode );
				}
			}
		} // namespace pathfinder
	}
}
