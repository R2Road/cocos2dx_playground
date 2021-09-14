#include "ui_research_text_field_BMFontScene.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIButton.h"
#include "ui/UITextField.h"
#include "ui/UIScale9Sprite.h"

#include "cpg_SStream.h"

USING_NS_CC;

namespace ui_research
{
	namespace text_field
	{
		BMFontScene::BMFontScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* BMFontScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BMFontScene( back_to_the_previous_scene_callback );
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

		bool BMFontScene::init()
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

				auto label = Label::createWithBMFont( "fonts/script_KR.fnt", ss.str(), TextHAlignment::LEFT );
				label->getFontAtlas()->setAliasTexParameters();
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setScale( _director->getContentScaleFactor() );
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
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Important Explain
			//
			{
				auto label = Label::createWithBMFont( "fonts/script_KR.fnt", "Font Size Must Be 0", TextHAlignment::LEFT );
				label->getFontAtlas()->setAliasTexParameters();
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::MAGENTA );
				label->setScale( _director->getContentScaleFactor() * 2 );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}



			const int TEXT_FIELD_MAX_LENGTH = 20;
			const char TEXT_FIELD_CURSOR_CHAR = 95; // "_"
			const std::string DUMMY_STRING( TEXT_FIELD_MAX_LENGTH, 'A' );
			const std::string PLACE_HOLDER_STRING( "input here~!" );

			//
			// Text Field : Original
			//
			{
				auto summury_for_text_field = Label::createWithBMFont( "fonts/script_KR.fnt", "Text Field Only : Has Some Problem\n Input Text > All Clear > Click Somewhere Else" );
				summury_for_text_field->setScale( _director->getContentScaleFactor() );
				summury_for_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				addChild( summury_for_text_field, 1 );

				auto ui_text_field = ui::TextField::create( DUMMY_STRING, "fonts/script_KR.fnt", 0 );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setMaxLength( TEXT_FIELD_MAX_LENGTH );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( TEXT_FIELD_CURSOR_CHAR );
				ui_text_field->setCursorEnabled( true );
				ui_text_field->setScale( _director->getContentScaleFactor() );
				ui_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.6f )
				) );
				addChild( ui_text_field, 0 );

				auto background_guide = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_2.png" );
				background_guide->setContentSize( ui_text_field->getBoundingBox().size + Size( 20.f, 10.f ) );
				background_guide->setPosition( ui_text_field->getPosition() );
				addChild( background_guide, 1 );

				ui_text_field->setPlaceHolder( PLACE_HOLDER_STRING );
			}

			//
			// Text Field : Suggest
			//
			{
				auto summury_for_text_field = Label::createWithBMFont( "fonts/script_KR.fnt", "Text Field + Button Support : Suggest" );
				summury_for_text_field->setScale( _director->getContentScaleFactor() );
				summury_for_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( summury_for_text_field, 1 );

				auto ui_text_field = ui::TextField::create( DUMMY_STRING, "fonts/script_KR.fnt", 0 );
				ui_text_field->setPlaceHolderColor( Color3B::GREEN );
				ui_text_field->setTextColor( Color4B::RED );
				ui_text_field->setMaxLength( TEXT_FIELD_MAX_LENGTH );
				ui_text_field->setMaxLengthEnabled( true );
				ui_text_field->setCursorChar( TEXT_FIELD_CURSOR_CHAR );
				ui_text_field->setCursorEnabled( true );
				ui_text_field->setScale( _director->getContentScaleFactor() );
				ui_text_field->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.2f )
				) );
				addChild( ui_text_field, 0 );

				auto guide_button = ui::Button::create( "guide_01_2.png", "guide_01_1.png", "guide_01_2.png", ui::Widget::TextureResType::PLIST );
				guide_button->setScale9Enabled( true );
				guide_button->setContentSize( ui_text_field->getBoundingBox().size + Size( 20.f, 10.f ) );
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

		void BMFontScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BMFontScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BMFontScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}


		void BMFontScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
