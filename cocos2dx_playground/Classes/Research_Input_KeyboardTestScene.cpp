#include "Research_Input_KeyboardTestScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"
#include "CPG_InputKeyViewer.h"

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

			std::stringstream ss;
			ss << "+ Keyboard Input Scene";
			ss << "\n";
			ss << "\n";
			ss << "[ESC] : Exit";
			ss << "\n";
			ss << "\n";
			ss << "[Arrow] : ...";

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
			label->setAnchorPoint( Vec2( 0.f, 1.f ) );
			label->setPosition( Vec2(
				origin.x
				, origin.y + visibleSize.height
			) );
			addChild( label, 9999 );


			//
			// input
			//
			auto input_delegator = CPG::Input::Delegator::create();
			addChild( input_delegator, 0 );

			key_map = CPG::Input::KeyMap::create( "datas/keyconfig/keymap_keyboardtest.json" );

			input_collector = CPG::Input::BasicCollector::create( key_map );
			input_delegator->addInputCollector( input_collector );


			//
			// key viewer
			//
			CPG::Input::KeyViewer* key_viewer = nullptr;
			key_viewer_start_position.set(
				origin.x + ( visibleSize.width * 0.5f )
				, origin.y + ( visibleSize.height * 0.1f )
			);
			for( int i = 0; i < key_viewer_count; ++i )
			{
				key_viewer = CPG::Input::KeyViewer::create( key_map );
				key_viewer->setPosition( key_viewer_start_position );
				key_viewer->setVisible( false );
				addChild( key_viewer, 1 );

				key_viewer_list.push_back( key_viewer );
			}
			key_viewer_end_position.set(
				key_viewer_start_position.x
				, key_viewer_start_position.y + ( key_viewer_list.front()->getContentSize().height * ( key_viewer_count - 1 ) )
			);

			return true;
		}

		void KeyboardTestScene::update( float dt )
		{
			if( input_collector->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
			{
				Director::getInstance()->replaceScene( RootScene::create() );
			}

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
	}
}
