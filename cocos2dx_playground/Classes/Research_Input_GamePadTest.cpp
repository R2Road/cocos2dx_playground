#include "Research_Input_GamePadTest.h"

#include <new>
#include <sstream>

#include "PlayGroundScene.h"

USING_NS_CC;

namespace
{
	const int TAG_KeyCodeViewNode = 20140416;
}

namespace research
{
	namespace input
	{
		GamePadTestScene::GamePadTestScene() :
			mPressedKeyCount( 0 )
			, mKeyboardListener( nullptr )
			, mControllerListener( nullptr )
		{}

		Scene* GamePadTestScene::create()
		{
			auto ret = new ( std::nothrow ) GamePadTestScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->scheduleUpdate();
				ret->autorelease();
			}

			return ret;
		}

		bool GamePadTestScene::init()
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
				auto background_layer = LayerColor::create( Color4B( 58, 0, 61, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// KeyCode View
			//
			{
				auto label = Label::createWithTTF( "Press Key", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_KeyCodeViewNode );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5, 0.5 ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, 9999 );

				clearKeyCodeView();
			}

			//
			// GamePad
			//
			{
				mControllerListener = EventListenerController::create();
				mControllerListener->onConnected= []( Controller*, Event* )->void {
					CCLOG( "Connected" );
				};
				mControllerListener->onDisconnected= []( Controller*, Event* )->void {
					CCLOG( "Disconnected" );
				};

				mControllerListener->onKeyDown = [this]( Controller*, int code, Event* )->void {
					++mPressedKeyCount;
					updateKeyCodeView( code );
				};

				mControllerListener->onKeyUp = [this]( Controller*, int code, Event* )->void {
					--mPressedKeyCount;
					if( 0 == mPressedKeyCount )
					{
						clearKeyCodeView();
					}
				};
				getEventDispatcher()->addEventListenerWithSceneGraphPriority( mControllerListener, this );

				//
				// Important
				//
				Controller::startDiscoveryController();
			}

			return true;
		}

		void GamePadTestScene::updateKeyCodeView( int keycode )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
			label->setString( std::to_string( static_cast<int>( keycode ) ) );
		}
		void GamePadTestScene::clearKeyCodeView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
			label->setString( "Press Key" );
		}

		void GamePadTestScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GamePadTestScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void GamePadTestScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Scene::onExit();
		}

		void GamePadTestScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( PlayGroundScene::create() );
		}
		void GamePadTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( !isScheduled( schedule_selector( GamePadTestScene::updateForExit ) ) )
				{
					scheduleOnce( schedule_selector( GamePadTestScene::updateForExit ), 0.f );
				}
			}
		}
	}
}
