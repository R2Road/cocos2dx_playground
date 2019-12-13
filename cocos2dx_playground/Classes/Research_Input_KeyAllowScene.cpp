#include "Research_Input_KeyAllowScene.h"

#include <sstream>

#include "ui/UICheckBox.h"

#include "RootScene.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
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
				auto checkbox = ui::CheckBox::create( "textures/ui/checkbox_normal.png", "textures/ui/checkbox_cross.png", ui::Widget::TextureResType::LOCAL );
				checkbox->getRendererBackground()->getTexture()->setAliasTexParameters();
				checkbox->getRendererFrontCross()->getTexture()->setAliasTexParameters();
				checkbox->setScale( 10.f );
				key_allow_control_root->addChild( checkbox, 1 );

				auto checkbox_label = Label::createWithTTF( "RIGHT_PARENTHESIS", "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
				checkbox_label->getFontAtlas()->setAliasTexParameters();
				key_allow_control_root->addChild( checkbox_label );
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

		void KeyAllowScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*_event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
				go_exit = true;
		}
	}
}
