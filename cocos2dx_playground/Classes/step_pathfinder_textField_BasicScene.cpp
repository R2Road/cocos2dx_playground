#include "step_pathfinder_textField_BasicScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "ui/UIButton.h"
#include "ui/UITextField.h"
#include "ui/UIScale9Sprite.h"

#include "step_pathfinder_RootScene.h"

USING_NS_CC;

namespace step_pathfinder
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
				auto background_layer = LayerColor::create( Color4B( 58, 0, 61, 255 ) );
				addChild( background_layer, -1 );
			}



			const int TEXT_FIELD_MAX_LENGTH = 20;
			const char TEXT_FIELD_CURSOR_CHAR = 95; // "_"
			const std::string DUMMY_STRING( TEXT_FIELD_MAX_LENGTH, 'A' );
			const std::string PLACE_HOLDER_STRING( "input here~!" );

			//
			// Text Field : Original
			//
			{
				auto summury_for_text_field = Label::createWithTTF( "Text Field Only : Has Some Problem\n Input Text > All Clear > Click Somewhere Else", "fonts/arial.ttf", 12 );
				summury_for_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.8f )
				) );
				addChild( summury_for_text_field, 1 );

				auto ui_text_field = ui::TextField::create( DUMMY_STRING, "fonts/arial.ttf", 12 );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setMaxLength( TEXT_FIELD_MAX_LENGTH );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( TEXT_FIELD_CURSOR_CHAR );
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

				ui_text_field->setPlaceHolder( PLACE_HOLDER_STRING );
			}

			//
			// Text Field : Suggest
			//
			{
				auto summury_for_text_field = Label::createWithTTF( "Text Field + Button Support : Suggest", "fonts/arial.ttf", 12 );
				summury_for_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.4f )
				) );
				addChild( summury_for_text_field, 1 );

				auto ui_text_field = ui::TextField::create( DUMMY_STRING, "fonts/arial.ttf", 12 );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setTextColor( Color4B::RED );
				ui_text_field->setMaxLength( TEXT_FIELD_MAX_LENGTH );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( TEXT_FIELD_CURSOR_CHAR );
				ui_text_field->setCursorEnabled( true );
				ui_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( ui_text_field, 0 );

				auto guide_button = ui::Button::create( "guide_01_4.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				guide_button->setScale9Enabled( true );
				guide_button->setContentSize( ui_text_field->getContentSize() + Size( 20.f, 10.f ) );
				guide_button->addTouchEventListener( [ui_text_field]( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type )
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
				guide_button->setPosition( ui_text_field->getPosition() );
				addChild( guide_button, 1 );

				ui_text_field->setPlaceHolder( PLACE_HOLDER_STRING );
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
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


		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step_pathfinder::RootScene::create() );
				return;
			}
		}
	}
}
