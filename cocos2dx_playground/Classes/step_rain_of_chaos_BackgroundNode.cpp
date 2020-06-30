#include "step_rain_of_chaos_BackgroundNode.h"

#include <array>
#include <new>
#include <numeric>
#include <random>
#include <utility>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteBatchNode.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

USING_NS_CC;

namespace
{
	const int TAG_BatchNode = 20140416;
}

namespace step_rain_of_chaos
{
	BackgroundNode::BackgroundNode( SpriteFrameContainerT&& sprite_frame_container ) : mSpriteFrameContainer( std::move( sprite_frame_container ) )
	{}

	BackgroundNode* BackgroundNode::create(
		const std::size_t vertical_amount
		, const std::size_t horizontal_amount
		, const char* texture_path
		, SpriteFrameContainerT&& sprite_frame_container
	)
	{
		CCASSERT( 0 != vertical_amount && 0 != horizontal_amount, "Failed - BackgroundNode::create" );
		CCASSERT( !sprite_frame_container.empty(), "Failed - BackgroundNode::create" );

		auto ret = new ( std::nothrow ) BackgroundNode( std::move( sprite_frame_container ) );
		if( !ret || !ret->init( vertical_amount, horizontal_amount, texture_path ) )
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

	bool BackgroundNode::init(
		const std::size_t vertical_amount
		, const std::size_t horizontal_amount
		, const char* texture_path
	)
	{
		if( !Node::init() )
		{
			return false;
		}

		//
		// Pivot
		//
		{
			auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
			pivot->setScale( 2.f );
			addChild( pivot, std::numeric_limits<int>::max() );
		}

		//
		// Generate Sprite Batch Node
		//
		{
			auto sprite_batch_node = SpriteBatchNode::create( texture_path, vertical_amount * horizontal_amount );
			sprite_batch_node->setTag( TAG_BatchNode );
			addChild( sprite_batch_node );
		}

		Reset( vertical_amount, horizontal_amount );

		return true;
	}

	void BackgroundNode::Reset( const std::size_t vertical_amount, const std::size_t horizontal_amount )
	{
		const Size SpriteSize = mSpriteFrameContainer[0]->getOriginalSizeInPixels();
		setContentSize( Size( SpriteSize.width * vertical_amount, SpriteSize.height * horizontal_amount ) );

		auto sprite_batch_node = static_cast<SpriteBatchNode*>( getChildByTag( TAG_BatchNode ) );
		sprite_batch_node->removeAllChildrenWithCleanup( false );

		// generate temp sprite
		Sprite* temp_sprite = Sprite::create();
		temp_sprite->setScale( _director->getContentScaleFactor() );

		std::mt19937 random_engine{ std::random_device{}( ) };
		std::uniform_int_distribution<> dist( 0, mSpriteFrameContainer.size() - 1 );

		auto sprite_frame_indicator = 0u;
		for( std::size_t sy = 0; horizontal_amount > sy; ++sy )
		{
			for( std::size_t sx = 0; vertical_amount > sx; ++sx )
			{
				// update target sprite frame
				sprite_frame_indicator = dist( random_engine );

				// sprite
				temp_sprite->setSpriteFrame( mSpriteFrameContainer[sprite_frame_indicator] );
				temp_sprite->setPosition(
					( SpriteSize.width * 0.5f ) + ( SpriteSize.width * sx )
					, ( SpriteSize.height * 0.5f ) + ( SpriteSize.height * sy )
				);
				sprite_batch_node->insertQuadFromSprite( temp_sprite, sx + ( horizontal_amount * sy ) );
			}
		}
	}
}
