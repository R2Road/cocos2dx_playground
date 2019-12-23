#include "Research_Input_KeyConfigScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
		namespace
		{
			const int TAG_KeyConfigControl_BG = 20140416;

			Node* createKeyConfigControl( const Size _control_size, const std::string& _key_name, const int _key_idx, const ui::Widget::ccWidgetTouchCallback& _callback )
			{
				auto root = Node::create();
				root->setContentSize( _control_size );
				{
					auto label = Label::createWithTTF( _key_name, "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
					root->addChild( label, 2 );

					auto button = ui::Button::create( "textures/ui/guide_01_1.png", "textures/ui/guide_01_2.png", "textures/ui/guide_01_1.png", ui::Widget::TextureResType::LOCAL );
					button->setTag( static_cast<int>( _key_idx ) );
					button->getRendererNormal()->getTexture()->setAliasTexParameters();
					button->getRendererClicked()->getTexture()->setAliasTexParameters();
					button->getRendererDisabled()->getTexture()->setAliasTexParameters();
					button->setScale9Enabled( true );
					button->setContentSize( _control_size );
					button->addTouchEventListener( _callback );
					root->addChild( button, 1 );

					auto indicator = ui::Scale9Sprite::create( "textures/ui/guide_01_3.png" );
					indicator->setTag( TAG_KeyConfigControl_BG );
					indicator->setVisible( false );
					indicator->setContentSize( _control_size );
					root->addChild( indicator, 0 );
				}

				return root;
			}
		}

		Scene* KeyConfigScene::create()
		{
			auto ret = new ( std::nothrow ) KeyConfigScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const Vec2 origin = Director::getInstance()->getVisibleOrigin();

			//
			// summury
			//
			{
				std::stringstream ss;
				ss << "+ Input : Key Config Scene";
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Exit";
				ss << "\n";
				ss << "\n";
				ss << "<Config File Path> : " << cocos2d::FileUtils::getInstance()->getWritablePath();

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					origin.x
					, origin.y + visibleSize.height
				) );
				ret->addChild( label, 9999 );
			}

			//
			// input
			//
			{
				auto input_delegator = CPG::Input::Delegator::create( "research_input_allowedKeysTest_allowed_keys.json" );
				ret->addChild( input_delegator, 0 );

				auto key_map = CPG::Input::KeyMap::create( "research_input_keyconfigscene_keymap.json" );

				ret->input_collector = CPG::Input::BasicCollector::create( key_map );
				input_delegator->addInputCollector( ret->input_collector );
			}

			//
			// Setup Key Config Controls
			//
			{
				ret->keymap_config_helper.load( "research_input_keyconfigscene_keymap.json" );

				int count = 0;
				for( const auto& h : ret->keymap_config_helper.getContainer() )
				{
					auto control = createKeyConfigControl( Size( 60, 20 ), h.name, h.idx, CC_CALLBACK_2( KeyConfigScene::onKeyConfigControl, ret ) );
					control->setPosition( Vec2(
						origin.x + ( visibleSize.width * 0.5f )
						, origin.y + ( control->getContentSize().height * 0.5f ) + 4.f + ( control->getContentSize().height * count )
					) );
					ret->addChild( control );

					++count;
				}
			}


			ret->autorelease();
			ret->scheduleUpdate();

			return ret;
		}

		void KeyConfigScene::update( float dt )
		{
			if( input_collector->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
			{
				Director::getInstance()->replaceScene( RootScene::create() );
			}
		}

		void KeyConfigScene::onKeyConfigControl( Ref* _sender, ui::Widget::TouchEventType _touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED != _touch_event_type )
				return;

			auto button = static_cast<ui::Button*>( _sender );
			auto bg = button->getParent()->getChildByTag( TAG_KeyConfigControl_BG );
			bg->setVisible( !bg->isVisible() );
		}
	}
}
