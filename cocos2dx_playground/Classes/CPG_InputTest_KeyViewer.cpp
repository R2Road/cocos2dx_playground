#include "CPG_InputTest_KeyViewer.h"

#include <new>

#include "cocos2d.h"

#include "CPG_Input_BasicCollector.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"

USING_NS_CC;

namespace cpg
{
	namespace input_test
	{
		const float view_margin = 4.f;
		const int TAG_free_key = 9999;

		KeyViewer::KeyViewer() :
			mKeyViews()
			, mView_StartX( 0 )
			, mView_Size()
		{}

		KeyViewer* KeyViewer::create( const KeyMapConfigHelper& key_map_config_helper )
		{
			auto ret = new ( std::nothrow ) KeyViewer();
			if( !ret || !ret->init( key_map_config_helper ) )
			{
				delete ret;
				ret = nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool KeyViewer::init( const KeyMapConfigHelper& key_map_config_helper )
		{
			if( !Node::init() )
			{
				return false;
			}

			mView_Size = SpriteFrameCache::getInstance()->getSpriteFrameByName( "key_free.png" )->getOriginalSize();
			const Size content_size(
				( mView_Size.width * key_map_config_helper.getContainer().size() )
				+ ( view_margin * std::max( 0, static_cast<int>( key_map_config_helper.getContainer().size() ) - 1 ) )
				, mView_Size.height
			);

			const Size total_margin( 3, 3 );
			const Size total_size( content_size + total_margin + total_margin );
			const float view_start_y = total_size.height * 0.5f;
			mView_StartX = total_margin.width;

			mKeyViews.reserve( key_map_config_helper.getContainer().size() );
			int a_i = 0;
			for( const auto& k : key_map_config_helper.getContainer() )
			{
				if( k.mSpriteFrameName.empty() )
				{
					continue;
				}

				auto arrow_sprite = Sprite::createWithSpriteFrameName( k.mSpriteFrameName );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				arrow_sprite->setPosition( Vec2(
					mView_StartX + ( ( mView_Size.width + view_margin ) * a_i )
					, view_start_y
				) );
				addChild( arrow_sprite );

				mKeyViews.emplace_back( k.mIdx, arrow_sprite );

				++a_i;
			}

			setContentSize( total_size );

			// key free sprite
			{
				auto arrow_sprite = Sprite::createWithSpriteFrameName( "key_free.png" );
				arrow_sprite->setTag( TAG_free_key );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				arrow_sprite->setPosition( mView_StartX, view_start_y );
				arrow_sprite->setVisible( false );
				addChild( arrow_sprite );
			}

			return true;
		}

		void KeyViewer::setup( const cpg::input::KeyCollectorSp key_collector )
		{
			//
			// Show Collect Key
			//
			int v_i = 0;
			for( auto& v : mKeyViews )
			{
				v.mSprite->setVisible( key_collector->getKeyStatus( v.mKeyIndex ) );
				if( !v.mSprite->isVisible() )
				{
					continue;
				}
				
				v.mSprite->setPositionX( mView_StartX + ( ( mView_Size.width + view_margin ) * v_i ) );
				++v_i;
			}

			//
			// Check : Key Visiblity
			//
			auto free_key_sprite = getChildByTag( TAG_free_key );
			if( key_collector->hasChanged() && 0 == v_i )
			{
				free_key_sprite->setVisible( true );
				free_key_sprite->setPositionX( mView_StartX + ( ( mView_Size.width + view_margin ) * v_i ) );
			}
			else
			{
				free_key_sprite->setVisible( false );
			}
		}
	}
}