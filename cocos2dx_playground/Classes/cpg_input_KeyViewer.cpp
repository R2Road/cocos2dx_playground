#include "cpg_input_KeyViewer.h"

#include <new>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"

#include "cpg_node_PivotNode.h"

#include "cpg_input_BasicKeyCollector.h"
#include "cpg_input_KeyMapConfigHelper.h"

USING_NS_CC;

namespace
{
	const Size Total_Margin( 3.f, 3.f );
	const float Key_Margin = 4.f;
}

namespace cpg_input
{
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
			( mKeySize.width * key_map_config_helper.GetContainer().size() )
			+ ( Key_Margin * std::max( 0, static_cast<int>( key_map_config_helper.GetContainer().size() ) - 1 ) )
			, mKeySize.height
		);

		const Size total_size( content_size + Total_Margin + Total_Margin );
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
		mKeyViews.reserve( key_map_config_helper.GetContainer().size() );
		for( const auto& k : key_map_config_helper.GetContainer() )
		{
			if( k.SpriteFrameName.empty() )
			{
				continue;
			}

			auto key_sprite_node = Sprite::createWithSpriteFrameName( k.SpriteFrameName );
			key_sprite_node->setAnchorPoint( Vec2::ZERO );
			key_sprite_node->setPosition( Total_Margin.width, Total_Margin.height );
			addChild( key_sprite_node );

			mKeyViews.emplace_back( k.Index, key_sprite_node );
		}

		//
		// View : Free Key
		//
		{
			mFreeKeySprite = Sprite::createWithSpriteFrameName( "key_free.png" );
			mFreeKeySprite->setAnchorPoint( Vec2::ZERO );
			mFreeKeySprite->setPosition( Total_Margin.width, Total_Margin.height );
			mFreeKeySprite->setVisible( false );
			addChild( mFreeKeySprite );
		}

		return true;
	}

	void KeyViewer::Setup( const iKeyCollector& key_collector )
	{
		//
		// Show Collect Key
		//
		int visible_sequence = 0;
		for( auto& v : mKeyViews )
		{
			if( !key_collector.getKeyStatus( v.KeyIndex ) )
			{
				v.SpriteNode->setVisible( false );
			}
			else
			{
				v.SpriteNode->setVisible( true );
				v.SpriteNode->setPositionX(
					Total_Margin.width
					+ ( ( mKeySize.width + Key_Margin ) * visible_sequence )
				);

				++visible_sequence;
			}
		}

		//
		// Check : Key Visiblity
		//
		if( key_collector.hasChanged() && 0 == visible_sequence )
		{
			mFreeKeySprite->setVisible( true );
		}
		else
		{
			mFreeKeySprite->setVisible( false );
		}
	}
}