#include "CPG_InputTest_KeyViewer.h"

#include <new>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"

#include "cpg_node_PivotNode.h"

#include "CPG_Input_BasicCollector.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"

USING_NS_CC;

namespace cpg
{
	namespace input_test
	{
		const Size total_margin( 3.f, 3.f );
		const float view_margin = 4.f;

		KeyViewer::KeyViewer() :
			mKeyViews()
			, mFreeKeySprite( nullptr )

			, mKeySize()
		{}

		KeyViewer* KeyViewer::create( const Config& config, const KeyMapConfigHelper& key_map_config_helper )
		{
			auto ret = new ( std::nothrow ) KeyViewer();
			if( !ret || !ret->init( config, key_map_config_helper ) )
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

		bool KeyViewer::init( const Config& config, const KeyMapConfigHelper& key_map_config_helper )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Pivot
			//
			if( config.bShowPivot )
			{
				auto pivot_node = cpg_node::PivotNode::create();
				addChild( pivot_node, std::numeric_limits<int>::max() );
			}

			mKeySize = SpriteFrameCache::getInstance()->getSpriteFrameByName( "key_free.png" )->getOriginalSize();
			const Size content_size(
				( mKeySize.width * key_map_config_helper.getContainer().size() )
				+ ( view_margin * std::max( 0, static_cast<int>( key_map_config_helper.getContainer().size() ) - 1 ) )
				, mKeySize.height
			);

			const Size total_size( content_size + total_margin + total_margin );
			setContentSize( total_size );

			//
			// Background
			//
			if( config.bShowBackground )
			{
				auto layer = LayerColor::create( Color4B( 40, 40, 40, 255 ), getContentSize().width, getContentSize().height );
				addChild( layer, std::numeric_limits<int>::min() );
			}

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
				arrow_sprite->setAnchorPoint( Vec2::ZERO );
				arrow_sprite->setPosition( total_margin.width, total_margin.height );
				addChild( arrow_sprite );

				mKeyViews.emplace_back( k.mIdx, arrow_sprite );
			}

			//
			// View : Free Key
			//
			{
				mFreeKeySprite = Sprite::createWithSpriteFrameName( "key_free.png" );
				mFreeKeySprite->setAnchorPoint( Vec2::ZERO );
				mFreeKeySprite->setPosition( total_margin.width, total_margin.height );
				mFreeKeySprite->setVisible( false );
				addChild( mFreeKeySprite );
			}

			return true;
		}

		void KeyViewer::Setup( const cpg::input::KeyCollectorSp key_collector )
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
					v.mSprite->setPositionX(
						total_margin.width
						+ ( ( mKeySize.width + view_margin ) * visible_sequence )
					);

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