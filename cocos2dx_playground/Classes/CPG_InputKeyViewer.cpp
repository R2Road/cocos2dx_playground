#include "CPG_InputKeyViewer.h"

#include "CPG_InputKeyMap.h"
#include "CPG_InputAnalyzer.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		KeyViewer::KeyViewer() : key_views() {}

		KeyViewer* KeyViewer::create( const KeyMapSp& key_map )
		{
			auto ret = new ( std::nothrow ) KeyViewer();
			if( !ret || !ret->init( key_map ) )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->autorelease();
			return ret;
		}

		bool KeyViewer::init( const KeyMapSp& key_map )
		{
			Node::init();

			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
			key_views.reserve( arrow_sprite_count );
			for( const auto& a : arrow_data_list )
			{
				auto arrow_sprite = Sprite::create( a.sprite_path );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				addChild( arrow_sprite );

				key_views.emplace_back( key_map->getKeyIndex( a.key_code ), arrow_sprite );
			}

			const float a_margin = 4.f;
			const auto a_size = key_views[0].sprite->getContentSize();
			const float a_total_width =
				( key_views[0].sprite->getContentSize().width * arrow_sprite_count )
				+ ( a_margin * std::max( 0, arrow_sprite_count - 1 ) );

			const float a_start_w = origin.x + ( visibleSize.width * 0.5f ) - ( a_total_width * 0.5f );
			const float a_start_h = origin.y + visibleSize.height * 0.5f;
			for( int a_i = 0; a_i < arrow_sprite_count; ++a_i )
			{
				key_views[a_i].sprite->setPosition( Vec2(
					a_start_w + ( ( a_size.width + a_margin ) * a_i )
					, a_start_h
				) );
			}

			return true;
		}

		void KeyViewer::setup( const CPG::Input::AnalyzerSp input_analyzer )
		{
			for( auto& a : key_views )
				a.sprite->setVisible( input_analyzer->getKeyStatus( a.key_index ) );
		}
	}
}