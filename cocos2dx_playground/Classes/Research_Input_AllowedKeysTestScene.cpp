#include "Research_Input_AllowedKeysTestScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
		const int key_viewer_count = 10;
		const float key_viewer_margin = 4.f;

		AllowedKeysTestScene::AllowedKeysTestScene() : input_delegator( nullptr ) {}

		Scene* AllowedKeysTestScene::create()
		{
			auto ret = new ( std::nothrow ) AllowedKeysTestScene();
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

		bool AllowedKeysTestScene::init()
		{
			if( !Scene::init() )
				return false;

			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

			std::stringstream ss;
			ss << "+ Allowed Keys Test Scene";
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


			//
			// input
			//
			input_delegator = CPG::Input::Delegator::create();
			addChild( input_delegator, 0 );

			return true;
		}
		void AllowedKeysTestScene::update( float dt )
		{
			if( input_delegator->isActiveKey( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
			{
				Director::getInstance()->replaceScene( RootScene::create() );
			}

			Scene::update( dt );
		}
	}
}
