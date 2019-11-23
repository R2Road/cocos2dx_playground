#include "Research_KeyboardInputScene.h"

#include <sstream>

#include "CPG_InputDelegator.h"

USING_NS_CC;

namespace Research
{
	KeyboardInputScene::KeyboardInputScene() : input_delegator( nullptr ) {}

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
		ret->input_delegator = CPG::Input::Delegator::create();
		ret->addChild( ret->input_delegator, 0 );


		ret->autorelease();
		ret->scheduleUpdate();

		return ret;
	}

	void KeyboardInputScene::update( float dt )
	{
		if( input_delegator->keyPressed_ESC() )
		{
			cocos2d::Director::getInstance()->end();
		}

		Scene::update( dt );
	}
}
