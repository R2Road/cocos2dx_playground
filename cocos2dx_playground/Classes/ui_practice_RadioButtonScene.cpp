#include "ui_practice_RadioButtonScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "ui/UIRadioButton.h"

USING_NS_CC;

namespace
{
	const int TAG_StatusView = 20140416;

	const char* FontPath = "fonts/NanumSquareR.ttf";
	const int FontSize = 9;
}

namespace ui_practice
{
	RadioButtonScene::RadioButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* RadioButtonScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) RadioButtonScene( back_to_the_previous_scene_callback );
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

	bool RadioButtonScene::init()
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

			auto label = Label::createWithTTF( ss.str(), FontPath, FontSize, Size::ZERO, TextHAlignment::LEFT );
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
			auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
			addChild( background_layer, std::numeric_limits<int>::min() );
		}

		//
		// Status View
		//
		{
			auto label = Label::createWithTTF( "", FontPath, FontSize );
			label->setTag( TAG_StatusView );
			label->setColor( Color3B::GREEN );
			label->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width * 0.5f
				, visibleOrigin.y + visibleSize.height * 0.7f
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Practice
		//
		{
			//
			// Group
			//
			auto radio_button_group_node = ui::RadioButtonGroup::create();
			radio_button_group_node->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.4f )
			) );
			addChild( radio_button_group_node );

			//
			// Radio Button 1
			//
			{
				// Radio Button
				auto radio_button = ui::RadioButton::create( "radio_button_01_0.png", "radio_button_01_1.png", "radio_button_01_3.png", "radio_button_01_2.png", "radio_button_01_4.png", ui::Widget::TextureResType::PLIST );
				radio_button->setTag( 0 );
				radio_button->setScale( _director->getContentScaleFactor() );
				radio_button->setPositionX( -visibleSize.width * 0.1f );
				radio_button->addTouchEventListener( CC_CALLBACK_2( RadioButtonScene::onRadioButton, this ) );
				radio_button_group_node->addChild( radio_button );

				radio_button_group_node->addRadioButton( radio_button );

				// Label
				auto label = Label::createWithTTF( "Test 1", FontPath, FontSize );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( radio_button->getPosition() + Vec2( 0.f, label->getContentSize().height + 4.f ) );
				radio_button_group_node->addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Radio Button 2
			//
			{
				// Radio Button
				auto radio_button = ui::RadioButton::create( "radio_button_01_0.png", "radio_button_01_1.png", "radio_button_01_3.png", "radio_button_01_2.png", "radio_button_01_4.png", ui::Widget::TextureResType::PLIST );
				radio_button->setTag( 1 );
				radio_button->setScale( _director->getContentScaleFactor() );
				radio_button->setPositionX( -visibleSize.width * 0.f );
				radio_button->addTouchEventListener( CC_CALLBACK_2( RadioButtonScene::onRadioButton, this ) );
				radio_button_group_node->addChild( radio_button );

				radio_button_group_node->addRadioButton( radio_button );

				// Label
				auto label = Label::createWithTTF( "Test 2", FontPath, FontSize );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( radio_button->getPosition() + Vec2( 0.f, label->getContentSize().height + 4.f ) );
				radio_button_group_node->addChild( label, std::numeric_limits<int>::max() );
			}

			// Radio Button 3
			{
				// Radio Button
				auto radio_button = ui::RadioButton::create( "radio_button_01_0.png", "radio_button_01_1.png", "radio_button_01_3.png", "radio_button_01_2.png", "radio_button_01_4.png", ui::Widget::TextureResType::PLIST );
				radio_button->setTag( 2 );
				radio_button->setScale( _director->getContentScaleFactor() );
				radio_button->setPositionX( visibleSize.width * 0.1f );
				radio_button->addTouchEventListener( CC_CALLBACK_2( RadioButtonScene::onRadioButton, this ) );
				radio_button_group_node->addChild( radio_button );

				radio_button_group_node->addRadioButton( radio_button );

				// Label
				auto label = Label::createWithTTF( "Test 3", FontPath, FontSize );
				label->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				label->setPosition( radio_button->getPosition() + Vec2( 0.f, label->getContentSize().height + 4.f ) );
				radio_button_group_node->addChild( label, std::numeric_limits<int>::max() );
			}

			radio_button_group_node->setSelectedButton( 2 );
			updateStatusView( 2 );
		}

		return true;
	}

	void RadioButtonScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RadioButtonScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void RadioButtonScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void RadioButtonScene::onRadioButton( Ref* sender, ui::Widget::TouchEventType touchEventType )
	{
		if( cocos2d::ui::Widget::TouchEventType::ENDED == touchEventType )
		{
			auto button_node = static_cast<Node*>( sender );
			updateStatusView( button_node->getTag() );
		}
	}
	void RadioButtonScene::updateStatusView( const int radio_button_tag )
	{
		auto label = static_cast<Label*>( getChildByTag( TAG_StatusView ) );
		label->setString( StringUtils::format( "Select : %d", radio_button_tag ) );
	}

	void RadioButtonScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
