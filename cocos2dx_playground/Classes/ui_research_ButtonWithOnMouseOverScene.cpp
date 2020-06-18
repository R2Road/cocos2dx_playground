#include "ui_research_ButtonWithOnMouseOverScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

USING_NS_CC;

namespace ui_research
{
	ButtonWithOnMouseOverScene::ButtonWithOnMouseOverScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mMouseListener( nullptr )
	{}

	Scene* ButtonWithOnMouseOverScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) ButtonWithOnMouseOverScene( back_to_the_previous_scene_callback );
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

	bool ButtonWithOnMouseOverScene::init()
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
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 50, 75, 112, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Research
		//
		{
			auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
			button->setScale9Enabled( true );
			button->setContentSize( Size( 100.f, 100.f ) );
			button->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			button->addTouchEventListener( CC_CALLBACK_2( ButtonWithOnMouseOverScene::onButton, this ) );
			addChild( button );
		}

		return true;
	}

	void ButtonWithOnMouseOverScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ButtonWithOnMouseOverScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );

		assert( !mMouseListener );
		mMouseListener = EventListenerMouse::create();
		mMouseListener->onMouseMove = []( EventMouse* event ) {
			CCLOG( "mouse move" );

			event->stopPropagation();
		};
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mMouseListener, this );
	}
	void ButtonWithOnMouseOverScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		assert( mMouseListener );
		getEventDispatcher()->removeEventListener( mMouseListener );
		mMouseListener = nullptr;

		Node::onExit();
	}

	void ButtonWithOnMouseOverScene::onButton( Ref* /*sender*/, ui::Widget::TouchEventType touch_event_type )
	{
		if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
		{
			CCLOG( "on button" );
		}
	}
	void ButtonWithOnMouseOverScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
