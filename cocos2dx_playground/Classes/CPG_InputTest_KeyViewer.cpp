#include "CPG_InputTest_KeyViewer.h"

#include "CPG_Input_BasicCollector.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"

USING_NS_CC;

namespace cpg
{
	namespace InputTest
	{
		const float view_margin = 4.f;
		const int TAG_free_key = 9999;

		KeyViewer::KeyViewer() :
			key_views()
			, view_start_x( 0 )
			, view_size()
		{}

		KeyViewer* KeyViewer::create( const KeyMapConfigHelper& _key_map_config_helper )
		{
			auto ret = new ( std::nothrow ) KeyViewer();
			if( !ret || !ret->init( _key_map_config_helper ) )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			ret->autorelease();
			return ret;
		}

		bool KeyViewer::init( const KeyMapConfigHelper& _key_map_config_helper )
		{
			Node::init();

			view_size = SpriteFrameCache::getInstance()->getSpriteFrameByName( "key_free.png" )->getOriginalSize();
			const Size content_size(
				( view_size.width * _key_map_config_helper.getContainer().size() )
				+ ( view_margin * std::max( 0, static_cast<int>( _key_map_config_helper.getContainer().size() ) - 1 ) )
				, view_size.height
			);

			const Size total_margin( 3, 3 );
			const Size total_size( content_size + total_margin + total_margin );
			const float view_start_y = total_size.height * 0.5f;
			view_start_x = total_margin.width;

			key_views.reserve( _key_map_config_helper.getContainer().size() );
			int a_i = 0;
			for( const auto& k : _key_map_config_helper.getContainer() )
			{
				if( k.sprite_frame_name.empty() )
					continue;

				auto arrow_sprite = Sprite::createWithSpriteFrameName( k.sprite_frame_name );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				arrow_sprite->setPosition( Vec2(
					view_start_x + ( ( view_size.width + view_margin ) * a_i )
					, view_start_y
				) );
				addChild( arrow_sprite );

				key_views.emplace_back( k.idx, arrow_sprite );

				++a_i;
			}

			setContentSize( total_size );

			// key free sprite
			{
				auto arrow_sprite = Sprite::createWithSpriteFrameName( "key_free.png" );
				arrow_sprite->setTag( TAG_free_key );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				arrow_sprite->setPosition( view_start_x, view_start_y );
				arrow_sprite->setVisible( false );
				addChild( arrow_sprite );
			}

			return true;
		}

		void KeyViewer::setup( const cpg::input::KeyCollectorSp key_collector )
		{
			int v_i = 0;
			for( auto& v : key_views )
			{
				v.sprite->setVisible( key_collector->getKeyStatus( v.key_index ) );
				if( !v.sprite->isVisible() )
					continue;
				
				v.sprite->setPositionX( view_start_x + ( ( view_size.width + view_margin ) * v_i ) );
				++v_i;
			}

			auto free_key_sprite = getChildByTag( TAG_free_key );
			if( key_collector->hasChanged() && 0 == v_i )
			{
				free_key_sprite->setVisible( true );
				free_key_sprite->setPositionX( view_start_x + ( ( view_size.width + view_margin ) * v_i ) );
			}
			else
				free_key_sprite->setVisible( false );
		}
	}
}