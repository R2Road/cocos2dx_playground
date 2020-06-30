#include "step_rain_of_chaos_BackgroundNode.h"

#include <array>
#include <new>
#include <numeric>
#include <random>

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

namespace step_rain_of_chaos
{
	BackgroundNode::BackgroundNode()
	{}

	BackgroundNode* BackgroundNode::create(
		const std::size_t vertical_amount
		, const std::size_t horizontal_amount
		, const char* texture_path
		, const SpriteFrameContainerT& sprite_frame_container
	)
	{
		CCASSERT( 0 != vertical_amount && 0 != horizontal_amount );
		CCASSERT( !sprite_frame_container.empty() );

		auto ret = new ( std::nothrow ) BackgroundNode();
		if( !ret || !ret->init( vertical_amount, horizontal_amount, texture_path, sprite_frame_container ) )
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
		, const SpriteFrameContainerT& sprite_frame_container
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

		const Size SpriteSize = sprite_frame_container[0]->getOriginalSizeInPixels();
		setContentSize( Size( SpriteSize.width * vertical_amount, SpriteSize.height * horizontal_amount ) );

		//
		// Batch Node
		//
		{
			// generate sprite batch node
			auto sprite_batch_node = SpriteBatchNode::create( texture_path, vertical_amount * horizontal_amount );
			sprite_batch_node->setContentSize( getContentSize() );
			addChild( sprite_batch_node );

			//
			// Build
			//
			{
				// generate temp sprite
				Sprite* temp_sprite = Sprite::create();
				temp_sprite->setScale( _director->getContentScaleFactor() );

				std::mt19937 random_engine{ std::random_device{}( ) };
				std::uniform_int_distribution<> dist( 0, sprite_frame_container.size() - 1 );

				auto sprite_frame_indicator = 0u;
				for( std::size_t sy = 0; horizontal_amount > sy; ++sy )
				{
					for( std::size_t sx = 0; vertical_amount > sx; ++sx )
					{
						// update target sprite frame
						sprite_frame_indicator = dist( random_engine );

						// sprite
						temp_sprite->setSpriteFrame( sprite_frame_container[sprite_frame_indicator] );
						temp_sprite->setPosition(
							( SpriteSize.width * 0.5f ) + ( SpriteSize.width * sx )
							, ( SpriteSize.height * 0.5f ) + ( SpriteSize.height * sy )
						);
						sprite_batch_node->insertQuadFromSprite( temp_sprite, sx + ( horizontal_amount * sy ) );
					}
				}
			}
		}

		return true;
	}
}
