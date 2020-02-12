#include "Step01_Game_PathFinder_ResultScene.h"

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
			ResultScene::ResultScene() : mKeyboardListener( nullptr ) {}

			Scene* ResultScene::create()
			{
				auto ret = new ( std::nothrow ) ResultScene();
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

			bool ResultScene::init()
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
				// Result
				//
				{
					auto label = Label::createWithTTF( "Game Clear", "fonts/arial.ttf", 20, Size::ZERO, TextHAlignment::CENTER );
					label->setColor( Color3B::BLACK );
					label->setPosition( Vec2(
						visibleOrigin.x + ( visibleSize.width * 0.5f )
						, visibleOrigin.y + ( visibleSize.height * 0.7f )
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
				Director::getInstance()->replaceScene( step01::game::pathfinder::TitleScene::create() );
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
		} // namespace pathfinder
	}
}
