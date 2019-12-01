#include "Research_KeyboardInputScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_InputAnalyzer.h"
#include "CPG_InputKeyMap.h"
#include "CPG_InputKeyViewer.h"

USING_NS_CC;

namespace Research
{
	const int key_viewer_count = 10;
	const float key_viewer_margin = 4.f;

	KeyboardInputScene::KeyboardInputScene() :
		key_map()
		, input_analyzer()
		, key_viewer_list()
		, key_viewer_start_position()
	{}

	Scene* KeyboardInputScene::create()
	{
		auto ret = new ( std::nothrow ) KeyboardInputScene();
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

	bool KeyboardInputScene::init()
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

		input_analyzer = CPG::Input::Analyzer::create( key_map );
		input_delegator->addAnalyzer( input_analyzer );


		//
		// key viewer
		//
		CPG::Input::KeyViewer* key_viewer = nullptr;
		key_viewer_start_position.set(
			origin.x
			, origin.y + ( visibleSize.height * 0.5f )
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

	void KeyboardInputScene::update( float dt )
	{
		if( input_analyzer->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}

		if( 0 != input_analyzer->getKeyStatusPackage() )
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
				v->setup( input_analyzer );
				break;
			}
		}

		Scene::update( dt );
	}
}
