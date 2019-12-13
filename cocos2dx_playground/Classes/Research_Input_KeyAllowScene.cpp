#include "Research_Input_KeyAllowScene.h"

#include <sstream>

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "RootScene.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
		namespace
		{
			const int TAG_KeyAllowControl_BG = 20140416;
		}

		KeyAllowScene::KeyAllowScene() :
			keyboard_listener( nullptr )
			, go_exit( false )
			, allowed_keys()
		{}

		Scene* KeyAllowScene::create()
		{
			auto ret = new ( std::nothrow ) KeyAllowScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->autorelease();
			ret->scheduleUpdate();

			return ret;
		}

		bool KeyAllowScene::init()
		{
			if( !Scene::init() )
				return false;

			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

			std::stringstream ss;
			ss << "+ Key Allow Scene";
			ss << "\n";
			ss << "\n";
			ss << "[ESC] : ...";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				origin.x
				, origin.y + visibleSize.height
			) );
			addChild( label, 9999 );

			allowed_keys = CPG::Input::AllowedKeys::load( "research_input_allowedKeysTest_allowed_keys.json" );


			auto key_allow_control_root = Node::create();
			key_allow_control_root->setPosition( Vec2(
				origin.x + ( visibleSize.width * 0.5f )
				, origin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( key_allow_control_root, 1 );
			{
				const Size key_allow_margin( 8.f, 4.f );

				auto key_allow_label = Label::createWithTTF( "RIGHT_PARENTHESIS", "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
				key_allow_label->getFontAtlas()->setAliasTexParameters();
				key_allow_control_root->addChild( key_allow_label, 2 );

				auto button = ui::Button::create( "textures/ui/guide_01_1.png", "textures/ui/guide_01_2.png", "textures/ui/guide_01_1.png", ui::Widget::TextureResType::LOCAL );
				button->getRendererNormal()->getTexture()->setAliasTexParameters();
				button->getRendererClicked()->getTexture()->setAliasTexParameters();
				button->getRendererDisabled()->getTexture()->setAliasTexParameters();
				button->setScale9Enabled( true );
				button->setContentSize( key_allow_label->getContentSize() + ( key_allow_margin * 2 ) );
				button->addTouchEventListener( CC_CALLBACK_2( KeyAllowScene::onKeyAllowControl, this ) );
				key_allow_control_root->addChild( button, 1 );

				auto bg = ui::Scale9Sprite::create( "textures/ui/guide_01_3.png" );
				bg->setTag( TAG_KeyAllowControl_BG );
				bg->setVisible( false );
				bg->setContentSize( button->getContentSize() );
				key_allow_control_root->addChild( bg, 0 );
			}

			return true;
		}

		void KeyAllowScene::onEnter()
		{
			Scene::onEnter();

			keyboard_listener = EventListenerKeyboard::create();
			keyboard_listener->onKeyReleased = CC_CALLBACK_2( KeyAllowScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( keyboard_listener, 1 );
		}
		void KeyAllowScene::update( float _dt )
		{
			if( go_exit )
			{
				go_exit = false;
				Director::getInstance()->replaceScene( RootScene::create() );
			}

			Node::update( _dt );
		}
		void KeyAllowScene::onExit()
		{
			if( keyboard_listener )
			{
				getEventDispatcher()->removeEventListener( keyboard_listener );
				keyboard_listener = nullptr;
			}
			Node::onExit();
		}

		void KeyAllowScene::onKeyAllowControl( Ref* _sender, ui::Widget::TouchEventType _touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED != _touch_event_type )
				return;

			auto button = static_cast<ui::Button*>( _sender );
			auto bg = button->getParent()->getChildByTag( TAG_KeyAllowControl_BG );
			bg->setVisible( !bg->isVisible() );
		}

		void KeyAllowScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
				go_exit = true;
		}
	}
}
