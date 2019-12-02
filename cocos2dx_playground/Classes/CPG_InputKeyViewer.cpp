#include "CPG_InputKeyViewer.h"

#include "CPG_InputKeyMap.h"
#include "CPG_Input_BasicCollector.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		const float view_margin = 4.f;

		KeyViewer::KeyViewer() :
			key_views()
			, view_start_x( 0 )
			, view_size()
		{}

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

			struct KeyViewConfig
			{
				cocos2d::EventKeyboard::KeyCode key_code;
				char* sprite_path;
			};
			const std::vector<KeyViewConfig> key_view_config_list( {
				{ cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW, "textures/keys/arrow_u.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW, "textures/keys/arrow_d.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW, "textures/keys/arrow_l.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW, "textures/keys/arrow_r.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_A, "textures/keys/key_a.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_B, "textures/keys/key_b.png" }
				,{ cocos2d::EventKeyboard::KeyCode::KEY_S, "textures/keys/key_s.png" }
			} );

			key_views.reserve( key_view_config_list.size() );
			for( const auto& a : key_view_config_list )
			{
				auto arrow_sprite = Sprite::create( a.sprite_path );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				addChild( arrow_sprite );

				key_views.emplace_back( key_map->getKeyIndex( a.key_code ), arrow_sprite );
			}

			view_size = key_views[0].sprite->getContentSize();
			const Size content_size(
				( key_views[0].sprite->getContentSize().width * key_view_config_list.size() )
				+ ( view_margin * std::max( 0, static_cast<int>( key_view_config_list.size() ) - 1 ) )
				, view_size.height
			);

			const Size total_margin( 3, 3 );
			const Size total_size( content_size + total_margin + total_margin );


			view_start_x = ( total_size.width * 0.5f ) - ( content_size.width * 0.5f );
			const float a_start_h = total_size.height * 0.5f;
			for( std::size_t a_i = 0, a_e = key_view_config_list.size(); a_i < a_e; ++a_i )
			{
				key_views[a_i].sprite->setPosition( Vec2(
					view_start_x + ( ( view_size.width + view_margin ) * a_i )
					, a_start_h
				) );
			}

			setContentSize( total_size );

			return true;
		}

		void KeyViewer::setup( const CPG::Input::BasicCollectorSp input_collector )
		{
			int v_i = 0;
			for( auto& v : key_views )
			{
				v.sprite->setVisible( input_collector->getKeyStatus( v.key_index ) );
				if( !v.sprite->isVisible() )
					continue;
				
				v.sprite->setPositionX( view_start_x + ( ( view_size.width + view_margin ) * v_i ) );
				++v_i;
			}
		}
	}
}