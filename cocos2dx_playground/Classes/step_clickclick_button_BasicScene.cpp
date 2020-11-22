#include "step_clickclick_button_BasicScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "ui/UIButton.h"

#include "cpg_StringTable.h"
#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_ButtonStatus = 20140416;
	const int TAG_TouchPosition = 20160528;
}

namespace step_clickclick
{
	namespace button
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr )
		{}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

		bool BasicScene::init()
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
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::WHITE );
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
				auto background_layer = LayerColor::create( Color4B( 0, 41, 13, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Button Status View
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10 );
				label->setTag( TAG_ButtonStatus );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				addChild( label );
			}

			//
			// Touch Info View
			//
			{
				auto label = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12 );
				label->setTag( TAG_TouchPosition );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.6f )
				) );
				addChild( label );
			}

			//
			// Button
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( Size( 50.f, 50.f ) );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				button->addTouchEventListener( CC_CALLBACK_2( BasicScene::onButton, this ) );
				addChild( button );

				auto label = Label::createWithTTF( "Click Here ===>>>", cpg::StringTable::GetFontPath(), 12, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 1.f, 0.5f ) );
				label->setPosition( button->getPosition() - Vec2( button->getContentSize().width * 0.7f, 0.f ) );
				addChild( label );

				onButton( button, cocos2d::ui::Widget::TouchEventType::ENDED );
			}

			

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void BasicScene::onButton( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			auto button = static_cast<ui::Button*>( sender );

			if( ui::Widget::TouchEventType::BEGAN == touch_event_type )
			{
				updateView_ButtonStatus( "Press" );
				updateView_TouchPosition( button->getTouchBeganPosition() );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				updateView_ButtonStatus( "Move" );
				updateView_TouchPosition( button->getTouchMovePosition() );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				updateView_ButtonStatus( "Release" );
				updateView_TouchPosition( button->getTouchEndPosition() );
			}
			else if( ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				updateView_ButtonStatus( "Release( Cancel )" );
			}
		}
		void BasicScene::updateView_ButtonStatus( const char* status_string )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_ButtonStatus ) );
			label->setString( status_string );
		}
		void BasicScene::updateView_TouchPosition( const cocos2d::Vec2 touch_position )
		{
			auto touch_position_label = static_cast<Label*>( getChildByTag( TAG_TouchPosition ) );
			touch_position_label->setString( StringUtils::format( "x : %f, y : %f", touch_position.x, touch_position.y ) );
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			_director->replaceScene( step_clickclick::RootScene::create() );
		}
		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				return;
			}

			if( isScheduled( schedule_selector( BasicScene::updateForExit ) ) )
			{
				return;
			}

			scheduleOnce( schedule_selector( BasicScene::updateForExit ), 0.f );
		}
	}
}
