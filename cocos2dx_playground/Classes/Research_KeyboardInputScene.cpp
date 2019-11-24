#include "Research_KeyboardInputScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_InputAnalyzer.h"

USING_NS_CC;

namespace Research
{
	KeyboardInputScene::KeyboardInputScene() :
		input_analyzer()
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


		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		std::stringstream ss;
		ss << "+ Keyboard Input Scene";
		ss << "\n";
		ss << "\n";
		ss << "[ESC] : Exit";

		auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
		label->setAnchorPoint( Vec2( 0.f, 1.f ) );
		label->setPosition( Vec2(
			origin.x
			, origin.y + visibleSize.height
		) );
		ret->addChild( label, 1 );


		//
		// input
		//
		auto input_delegator = CPG::Input::Delegator::create();
		ret->addChild( input_delegator, 0 );

		ret->input_analyzer = CPG::Input::Analyzer::create();
		input_delegator->addAnalyzer( ret->input_analyzer );


		ret->autorelease();
		ret->scheduleUpdate();

		return ret;
	}

	void KeyboardInputScene::update( float dt )
	{
		if( input_analyzer->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}

		Scene::update( dt );
	}
}
