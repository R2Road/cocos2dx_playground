#include "ui_practice_ToggleButtonScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "extensions/GUI/CCControlExtension/CCControlSwitch.h"
#include "extensions/GUI/CCControlExtension/CCInvocation.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"


USING_NS_CC;

namespace
{
	const int TAG_StatusView = 20140416;

	const int FontSize = 9;
}

namespace ui_practice
{
	ToggleButtonScene::ToggleButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
		helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
		, mKeyboardListener( nullptr )
	{}

	Scene* ToggleButtonScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
	{
		auto ret = new ( std::nothrow ) ToggleButtonScene( back_to_the_previous_scene_callback );
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			ret->autorelease();
		}

		return ret;
	}

	bool ToggleButtonScene::init()
	{
		if( !Scene::init() )
		{
			return false;
		}

		const auto visibleSize = _director->getVisibleSize();
		const auto visibleOrigin = _director->getVisibleOrigin();

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << cpg::linefeed;
			ss << cpg::linefeed;
			ss << "[ESC] : Return to Root";

			auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), FontSize, Size::ZERO, TextHAlignment::LEFT );
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
			auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), FontSize );
			label->setTag( TAG_StatusView );
			label->setColor( Color3B::GREEN );
			label->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width * 0.5f
				, visibleOrigin.y + visibleSize.height * 0.6f
			) );
			addChild( label, std::numeric_limits<int>::max() );
		}

		//
		// Practice
		//
		{
			auto mask_sprite = Sprite::create( "textures/toggle_button_mask_01_0.png" );
			mask_sprite->getTexture()->setAliasTexParameters();

			auto on_sprite = Sprite::createWithSpriteFrameName( "toggle_button_01_0.png" );
			auto off_sprite = Sprite::createWithSpriteFrameName( "toggle_button_01_1.png" );
			auto thumb_sprite = Sprite::createWithSpriteFrameName( "toggle_button_thumb_01_0.png" );

			auto control_switch_node = extension::ControlSwitch::create( mask_sprite, on_sprite, off_sprite, thumb_sprite );
			control_switch_node->setPosition( Vec2(
				visibleOrigin.x + visibleSize.width * 0.5f
				, visibleOrigin.y + visibleSize.height * 0.4f
			) );
			control_switch_node->setScale( _director->getContentScaleFactor() );
			control_switch_node->setOn( true );
			addChild( control_switch_node );

			//
			// Callback
			//
			control_switch_node->addTargetWithActionForControlEvents( this, cccontrol_selector( ToggleButtonScene::onToggleButton ), extension::Control::EventType::VALUE_CHANGED );

			//
			//
			//
			updateStatusView( control_switch_node->isOn() );
		}

		return true;
	}

	void ToggleButtonScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ToggleButtonScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void ToggleButtonScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}

	void ToggleButtonScene::onToggleButton( Ref* sender, extension::Control::EventType event_type )
	{
		if( extension::Control::EventType::VALUE_CHANGED == event_type ) // is need?
		{
			auto control_switch_node = static_cast<extension::ControlSwitch*>( sender );
			updateStatusView( control_switch_node->isOn() );
		}
	}
	void ToggleButtonScene::updateStatusView( const bool new_status )
	{
		auto label = static_cast<Label*>( getChildByTag( TAG_StatusView ) );
		label->setString( new_status ? "Is On" : "Is Off" );
	}

	void ToggleButtonScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
		{
			helper::BackToThePreviousScene::MoveBack();
			return;
		}
	}
}
