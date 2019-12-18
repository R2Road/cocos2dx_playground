#include "Research_Input_AllowedKeysTestScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_InputTest_KeyboardInputObserver.h"
#include "CPG_Input_KeyNames.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
		const int key_viewer_count = 10;
		const float key_viewer_margin = 4.f;

		AllowedKeysTestScene::AllowedKeysTestScene() :
			input_delegator( nullptr )
			, key_viewer( nullptr )
			, key_string( 200, 0 )
			, input_obserber( nullptr )
		{}

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
			input_delegator = CPG::Input::Delegator::create( "research_input_allowedKeysTest_allowed_keys.json" );
			addChild( input_delegator, 0 );

			input_obserber = CPG::InputTest::KeyboardInputObserber::create( "research_input_allowedKeysTest_allowed_keys.json" );
			addChild( input_obserber, 1 );


			//
			// key viewer
			//
			key_viewer = Label::createWithTTF( "", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
			key_viewer->setPosition( Vec2(
				origin.x + ( visibleSize.width * 0.5f )
				, origin.y + ( visibleSize.height * 0.5f )
			) );
			addChild( key_viewer, 9999 );

			return true;
		}
		void AllowedKeysTestScene::update( float dt )
		{
			if( input_obserber->inputFound() )
			{
				if( input_delegator->isActiveKey( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
				{
					Director::getInstance()->replaceScene( RootScene::create() );
				}

				key_string.clear();
				for( std::size_t cur = 0; cur < CPG::Input::AllowedKeys::ContainerSize; ++cur )
				{
					if( input_delegator->isActiveKey( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) ) )
					{
						key_string += CPG::Input::KeyNames::get( static_cast<cocos2d::EventKeyboard::KeyCode>( cur ) );
						key_string += "\n";
					}
				}
				key_viewer->setString( key_string );
			}

			Scene::update( dt );
		}
	}
}
