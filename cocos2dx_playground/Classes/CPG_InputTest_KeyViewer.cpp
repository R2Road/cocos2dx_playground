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

		KeyViewer::KeyViewer() :
			mKeyViews()
			, mFreeKeySprite( nullptr )

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
			setContentSize( total_size );

			const float view_start_y = total_size.height * 0.5f;
			mView_StartX = total_margin.width;

			//
			// View : Keys
			//
			mKeyViews.reserve( key_map_config_helper.getContainer().size() );
			for( const auto& k : key_map_config_helper.getContainer() )
			{
				if( k.mSpriteFrameName.empty() )
				{
					continue;
				}

				auto arrow_sprite = Sprite::createWithSpriteFrameName( k.mSpriteFrameName );
				arrow_sprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				arrow_sprite->setPosition( mView_StartX, view_start_y );
				addChild( arrow_sprite );

				mKeyViews.emplace_back( k.mIdx, arrow_sprite );
			}

			//
			// View : Free Key
			//
			{
				mFreeKeySprite = Sprite::createWithSpriteFrameName( "key_free.png" );
				mFreeKeySprite->setAnchorPoint( Vec2( 0.f, 0.5f ) );
				mFreeKeySprite->setPosition( mView_StartX, view_start_y );
				mFreeKeySprite->setVisible( false );
				addChild( mFreeKeySprite );
			}

			return true;
		}

		void KeyViewer::setup( const cpg::input::KeyCollectorSp key_collector )
		{
			//
			// Show Collect Key
			//
			int visible_sequence = 0;
			for( auto& v : mKeyViews )
			{
				if( !key_collector->getKeyStatus( v.mKeyIndex ) )
				{
					v.mSprite->setVisible( false );
				}
				else
				{
					v.mSprite->setVisible( true );
					v.mSprite->setPositionX( mView_StartX + ( ( mView_Size.width + view_margin ) * visible_sequence ) );

					++visible_sequence;
				}
			}

			//
			// Check : Key Visiblity
			//
			if( key_collector->hasChanged() && 0 == visible_sequence )
			{
				mFreeKeySprite->setVisible( true );
			}
			else
			{
				mFreeKeySprite->setVisible( false );
			}
		}
	}
}