#include "input_practice_GamePadTestScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCController.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerController.h"
#include "base/CCEventDispatcher.h"

USING_NS_CC;

namespace
{
	const int TAG_KeyCodeViewNode = 20140416;
}

namespace input_practice
{
	GamePadTestScene::GamePadTestScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mControllerListener( nullptr )
		, mPressedKeyCount( 0 )
	{}

	Scene* GamePadTestScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) GamePadTestScene( back_to_the_previous_scene_callback );
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
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x
				, visibleOrigin.y + visibleSize.height
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Explain
		//
		{
			std::stringstream ss;
			ss << "+ Test Environment";
			ss << std::endl;
			ss << "Win10 + Xbox Game Controller";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::RIGHT );
			label->setColor( Color3B::GREEN );
			label->setAnchorPoint( Vec2( 1.f, 1.f ) );
			label->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width
				, visibleOrigin.y + visibleSize.height
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 30,76, 59, 255 ) );
			addChild( background_layer, -1 );
		}

		//
		// KeyCode View
		//
		{
			auto label = Label::createWithTTF( "", "fonts/arial.ttf", 9 );
			label->setTag( TAG_KeyCodeViewNode );
			label->setColor( Color3B::GREEN );
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

			mControllerListener->onKeyUp = [this]( Controller*, int, Event* )->void {
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

	void GamePadTestScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
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

	void GamePadTestScene::updateKeyCodeView( const int keycode )
	{
		auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
		label->setString( std::to_string( static_cast<int>( keycode ) ) );
	}
	void GamePadTestScene::clearKeyCodeView()
	{
		auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
		label->setString( "Press Key" );
	}

	void GamePadTestScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			helper::BackToThePreviousScene::MoveBack();
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
}
