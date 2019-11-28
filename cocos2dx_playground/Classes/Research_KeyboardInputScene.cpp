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
	KeyboardInputScene::KeyboardInputScene() :
		key_map()
		, input_analyzer()
		, key_viewer()
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
		addChild( label, 1 );


		//
		// input
		//
		auto input_delegator = CPG::Input::Delegator::create();
		addChild( input_delegator, 0 );

		key_map = CPG::Input::KeyMap::create();

		input_analyzer = CPG::Input::Analyzer::create( key_map );
		input_delegator->addAnalyzer( input_analyzer );


		//
		// key viewer
		//
		key_viewer = CPG::Input::KeyViewer::create( key_map );
		key_viewer->setPosition(
			origin.x
			, origin.y + ( visibleSize.height * 0.5f )
		);
		addChild( key_viewer, 1 );


		return true;
	}

	void KeyboardInputScene::update( float dt )
	{
		if( input_analyzer->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}

		key_viewer->setup( input_analyzer );

		Scene::update( dt );
	}
}
