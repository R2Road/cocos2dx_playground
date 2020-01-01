#include "Research_Input_KeyboardTestScene.h"

#include <sstream>

#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"
#include "CPG_InputTest_KeyViewer.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"
#include "CPG_Input_KeyCodeNames.h"

#include "Research_Setting.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
		const int key_viewer_count = 10;
		const float key_viewer_margin = 4.f;

		KeyboardTestScene::KeyboardTestScene() :
			key_map()
			, input_collector()
			, key_viewer_list()
			, key_viewer_start_position()
		{}

		Scene* KeyboardTestScene::create()
		{
			auto ret = new ( std::nothrow ) KeyboardTestScene();
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

		bool KeyboardTestScene::init()
		{
			if( !Scene::init() )
				return false;

			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

			CPG::InputTest::KeyMapConfigHelper key_map_config_helper;
			key_map_config_helper.load( Research::Setting::getKeyMapFileName().c_str() );

			//
			// summury
			//
			{
				std::stringstream ss;
				ss << "+ Keyboard Input Scene";
				ss << "\n";
				ss << "\n";
				for( const auto& h : key_map_config_helper.getContainer() )
					ss << "[ " << h.name << " : " << CPG::Input::KeyCodeNames::get( h.keycode ) << " ]" << std::endl;

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					origin.x
					, origin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// exit interface
			//
			{
				auto label = Label::createWithTTF( "Exit", "fonts/arial.ttf", 10, Size::ZERO, TextHAlignment::CENTER );
				label->setColor( Color3B::GREEN );

				auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_1.png", ui::Widget::TextureResType::PLIST );
				button->setColor( Color3B::GREEN );
				button->getRendererNormal()->getTexture()->setAliasTexParameters();
				button->getRendererClicked()->getTexture()->setAliasTexParameters();
				button->getRendererDisabled()->getTexture()->setAliasTexParameters();
				button->setScale9Enabled( true );
				button->setContentSize( label->getContentSize() + Size( 40.f, 4.f ) + Size( 40.f, 4.f ) );
				button->addTouchEventListener( CC_CALLBACK_2( KeyboardTestScene::onExitButton, this ) );
				addChild( button, 9999 );
				button->setTitleLabel( label );

				button->setPosition( Vec2(
					origin.x + visibleSize.width - ( button->getContentSize().width * 0.5f )
					, origin.y + visibleSize.height - ( button->getContentSize().height * 0.5f )
				) );
			}


			//
			// input
			//
			{
				auto input_delegator = CPG::Input::Delegator::create( Research::Setting::getKeyAllowFileName().c_str() );
				addChild( input_delegator, 0 );

				key_map = CPG::Input::KeyMap::create( Research::Setting::getKeyMapFileName().c_str() );

				input_collector = CPG::Input::BasicCollector::create( key_map );
				input_delegator->addInputCollector( input_collector );
			}


			//
			// key viewer
			//
			{
				CPG::InputTest::KeyViewer* key_viewer = nullptr;
				key_viewer_start_position.set(
					origin.x + ( visibleSize.width * 0.5f )
					, origin.y + ( visibleSize.height * 0.1f )
				);
				for( int i = 0; i < key_viewer_count; ++i )
				{
					key_viewer = CPG::InputTest::KeyViewer::create( key_map );
					key_viewer->setPosition( key_viewer_start_position );
					key_viewer->setVisible( false );
					addChild( key_viewer, 1 );

					key_viewer_list.push_back( key_viewer );
				}
				key_viewer_end_position.set(
					key_viewer_start_position.x
					, key_viewer_start_position.y + ( key_viewer_list.front()->getContentSize().height * ( key_viewer_count - 1 ) )
				);
			}

			return true;
		}

		void KeyboardTestScene::update( float dt )
		{
			if( input_collector->hasChanged() )
			{
				for( auto v : key_viewer_list )
				{
					if( !v->isVisible() )
						continue;

					v->setPositionY( v->getPositionY() + v->getContentSize().height + key_viewer_margin );
					v->setVisible( key_viewer_end_position.y > v->getPositionY() );
				}

				for( auto v : key_viewer_list )
				{
					if( v->isVisible() )
						continue;

					v->setVisible( true );
					v->setPosition( key_viewer_start_position );
					v->setup( input_collector );
					break;
				}
			}

			Scene::update( dt );
		}

		void KeyboardTestScene::onExitButton( Ref* /*_sender*/, ui::Widget::TouchEventType _touch_event_type )
		{
			if( ui::Widget::TouchEventType::ENDED != _touch_event_type )
				return;

			if( !isScheduled( schedule_selector( KeyboardTestScene::update_forExit ) ) )
				scheduleOnce( schedule_selector( KeyboardTestScene::update_forExit ), 0.f );
		}
		void KeyboardTestScene::update_forExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}
	}
}
