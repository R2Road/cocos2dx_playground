#include "Research_KeyboardInputScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_InputAnalyzer.h"
#include "CPG_InputKeyMap.h"

USING_NS_CC;

namespace Research
{
	KeyboardInputScene::KeyboardInputScene() :
		key_map()
		, input_analyzer()
		, arrow_views()
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
		//
		//
		{
			const int arrow_sprite_count = 4;
			struct ArrowConfigData
			{
				cocos2d::EventKeyboard::KeyCode key_code;
				char* sprite_path;
			};
			const ArrowConfigData arrow_data_list[arrow_sprite_count] = {
				{ cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW, "arrow/arrow_u.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW, "arrow/arrow_d.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW, "arrow/arrow_l.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW, "arrow/arrow_r.png" }
			};
			arrow_views.reserve( arrow_sprite_count );
			for( const auto& a : arrow_data_list )
			{
				auto arrow_sprite = Sprite::create( a.sprite_path );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				addChild( arrow_sprite );

				arrow_views.emplace_back( key_map->getKeyIndex( a.key_code ), arrow_sprite );
			}

			const float a_margin = 4.f;
			const auto a_size = arrow_views[0].sprite->getContentSize();
			const float a_total_width =
				( arrow_views[0].sprite->getContentSize().width * arrow_sprite_count )
				+ ( a_margin * std::max( 0, arrow_sprite_count - 1 ) );

			const float a_start_w = origin.x + ( visibleSize.width * 0.5f ) - ( a_total_width * 0.5f );
			const float a_start_h = origin.y + visibleSize.height * 0.5f;
			for( int a_i = 0; a_i < arrow_sprite_count; ++a_i )
			{
				arrow_views[a_i].sprite->setPosition( Vec2(
					a_start_w + ( ( a_size.width + a_margin ) * a_i )
					, a_start_h
				) );
			}
		}

		return true;
	}

	void KeyboardInputScene::update( float dt )
	{
		if( input_analyzer->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}

		for( auto& a : arrow_views )
			a.sprite->setVisible( input_analyzer->getKeyStatus( a.key_index ) );

		Scene::update( dt );
	}
}
