#include "ui_research_ButtonWithOnMouseOverScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCCamera.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventDispatcher.h"
#include "ui/UIButton.h"

USING_NS_CC;

namespace
{
	const int TAG_button = 20140416;
	const int TAG_MouseStatusLabel = 20160528;
}

namespace ui_research
{
	ButtonWithOnMouseOverScene::ButtonWithOnMouseOverScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
		, mMouseListener( nullptr )
		, mbOnMouseOver( false )
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
			// Test Button
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setTag( TAG_button );
				button->setScale9Enabled( true );
				button->setContentSize( Size( 100.f, 100.f ) );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				button->addTouchEventListener( CC_CALLBACK_2( ButtonWithOnMouseOverScene::onButton, this ) );
				addChild( button );
			}

			// Mouse Status View
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 14, Size::ZERO, TextHAlignment::CENTER );
				label->setTag( TAG_MouseStatusLabel );
				label->setAnchorPoint( Vec2( 0.5f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.8f )
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateMouseStatusView( false );
			}
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
		mMouseListener->onMouseMove = [this]( EventMouse* event ) {

			//
			// 20200618
			// This code originated from "Widget::onTouchBegan"
			//

			const auto button = static_cast<ui::Button*>( getChildByTag( TAG_button ) );
			const auto camera = Camera::getVisitingCamera();
			const auto current_hit_result = button->hitTest( Vec2( event->getCursorX(), event->getCursorY() ), camera, nullptr );

			if( !mbOnMouseOver && current_hit_result )
			{
				updateMouseStatusView( current_hit_result );

				mbOnMouseOver = current_hit_result;
				event->stopPropagation();
			}
			else if( mbOnMouseOver && !current_hit_result )
			{
				updateMouseStatusView( current_hit_result );

				mbOnMouseOver = current_hit_result;
			}
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
	void ButtonWithOnMouseOverScene::updateMouseStatusView( bool is_over )
	{
		auto label = static_cast<Label*>( getChildByTag( TAG_MouseStatusLabel ) );
		if( is_over )
		{
			label->setString( "Over" );
		}
		else
		{
			label->setString( "Out" );
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
