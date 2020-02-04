#include "Step01_TextField_BasicScene.h"

#include <new>
#include <sstream>

#include "ui/UIButton.h"
#include "ui/UITextField.h"
#include "ui/UIScale9Sprite.h"

#include "Step01_RootScene.h"

USING_NS_CC;

namespace step01
{
	namespace text_field
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
				return nullptr;
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
				return false;

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
				ss << "[ESC] : Return to Step01 Root";

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
			// Text Field : Original
			//
			{
				auto label = Label::createWithTTF( "Text Field Original : Has Some Problem\n Input Text > All Clear > Click Somewhere Else", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.8f )
				) );
				addChild( label, 1 );

				auto ui_text_field = ui::TextField::create( "input text here~!!!!", "fonts/arial.ttf", 9 );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setMaxLength( 20 );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( 95 );
				ui_text_field->setCursorEnabled( true );
				ui_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				addChild( ui_text_field, 0 );

				auto background_guide = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_4.png" );
				background_guide->setContentSize( ui_text_field->getContentSize() + Size( 20.f, 10.f ) );
				background_guide->setPosition( ui_text_field->getPosition() );
				addChild( background_guide, 1 );
			}

			//
			// Text Field : Suggest
			//
			{
				auto label = Label::createWithTTF( "Text Field + Button Support : Suggest", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::CENTER );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.4f )
				) );
				addChild( label, 1 );

				auto ui_text_field = ui::TextField::create( "input text here!!!!!", "fonts/arial.ttf", 9 );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setTextColor( Color4B::RED );
				ui_text_field->setMaxLength( 20 );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( 95 );
				ui_text_field->setCursorEnabled( true );
				ui_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( ui_text_field, 0 );

				auto background_guide = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				background_guide->setScale9Enabled( true );
				background_guide->setContentSize( ui_text_field->getContentSize() + Size( 20.f, 10.f ) );
				background_guide->addTouchEventListener( [ui_text_field]( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type )
				{
					switch( touch_event_type )
					{
					case cocos2d::ui::Widget::TouchEventType::CANCELED:
					case cocos2d::ui::Widget::TouchEventType::ENDED:
					{
						ui_text_field->attachWithIME();
					}
					break;
					}
				} );
				background_guide->setPosition( ui_text_field->getPosition() );
				addChild( background_guide, 1 );
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}


		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
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
