#include "Research_Input_KeyConfigScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
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

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto input_delegator = CPG::Input::Delegator::create();
				ret->addChild( input_delegator, 0 );

				auto key_map = CPG::Input::KeyMap::create( "" );

				ret->input_analyzer = CPG::Input::Analyzer::create( key_map );
				input_delegator->addAnalyzer( ret->input_analyzer );
			}


			ret->autorelease();
			ret->scheduleUpdate();

			return ret;
		}

		void KeyConfigScene::update( float dt )
		{
			if( input_analyzer->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
			{
				Director::getInstance()->replaceScene( RootScene::create() );
			}
		}
	}
}
