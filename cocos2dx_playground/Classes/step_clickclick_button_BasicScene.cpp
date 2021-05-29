#include "step_clickclick_button_BasicScene.h"

#include <new>
#include <numeric>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"
#include "ui/UIButton.h"

#include "cpg_SStream.h"
#include "cpg_StringTable.h"
#include "step_clickclick_RootScene.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace button
	{
		BasicScene::BasicScene() :
			mKeyboardListener( nullptr )

			, mButtonStatusLabel( nullptr )
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
				ss << cpg::linefeed;
				ss << cpg::linefeed;
				ss << "[ESC] : Return to Root";

				auto label = Label::createWithTTF( ss.str(), cpg::StringTable::GetFontPath(), 10, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::WHITE );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition(
					visibleOrigin
					+ Vec2( 0.f, visibleSize.height )
				);
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
				mButtonStatusLabel = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 10 );
				mButtonStatusLabel->setColor( Color3B::GREEN );
				mButtonStatusLabel->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.7f )
				);
				addChild( mButtonStatusLabel );
			}

			//
			// Touch Info View
			//
			{
				mTouchInfoLabel = Label::createWithTTF( "", cpg::StringTable::GetFontPath(), 12 );
				mTouchInfoLabel->setColor( Color3B::GREEN );
				mTouchInfoLabel->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.6f )
				);
				addChild( mTouchInfoLabel );
			}

			//
			// Button
			//
			{
				auto button = ui::Button::create( "guide_01_0.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				button->setScale9Enabled( true );
				button->setContentSize( Size( 50.f, 50.f ) );
				button->setPosition(
					visibleOrigin
					+ Vec2( visibleSize.width * 0.5f, visibleSize.height * 0.3f )
				);
				button->addTouchEventListener( CC_CALLBACK_2( BasicScene::onButton, this ) );
				addChild( button );

				auto label = Label::createWithTTF( "Click Here ===>>>", cpg::StringTable::GetFontPath(), 12 );
				label->setColor( Color3B::RED );
				label->setAnchorPoint( Vec2( 1.f, 0.5f ) );
				label->setPosition(
					button->getPosition()
					- Vec2( button->getContentSize().width * 0.7f, 0.f )
				);
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
				mButtonStatusLabel->setString( "Press" );
				updateView_TouchPosition( button->getTouchBeganPosition() );
			}
			else if( ui::Widget::TouchEventType::MOVED == touch_event_type )
			{
				mButtonStatusLabel->setString( "Move" );
				updateView_TouchPosition( button->getTouchMovePosition() );
			}
			else if( ui::Widget::TouchEventType::ENDED == touch_event_type )
			{
				mButtonStatusLabel->setString( "Release" );
				updateView_TouchPosition( button->getTouchEndPosition() );
			}
			else if( ui::Widget::TouchEventType::CANCELED == touch_event_type )
			{
				mButtonStatusLabel->setString( "Release( Cancel )" );
			}
		}
		void BasicScene::updateView_TouchPosition( const cocos2d::Vec2 touch_position )
		{
			mTouchInfoLabel->setString( StringUtils::format( "x : %f, y : %f", touch_position.x, touch_position.y ) );
		}


		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_clickclick::RootScene::create() );
				return;
			}
		}
	}
}
