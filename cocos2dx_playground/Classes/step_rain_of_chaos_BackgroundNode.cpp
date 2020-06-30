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

	BackgroundNode* BackgroundNode::create()
	{
		auto ret = new ( std::nothrow ) BackgroundNode();
		if( !ret || !ret->init() )
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

	bool BackgroundNode::init()
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

		const std::array<SpriteFrame*, 2u> SpriteFrames{
				SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_tile_0.png" )
				,SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_tile_1.png" )
		};
		const Size SpriteSize = SpriteFrames[0]->getOriginalSizeInPixels();
		const int VerticalAmount = 7;
		const int HorizontalAmount = 7;

		setContentSize( Size( SpriteSize.width * VerticalAmount, SpriteSize.height * HorizontalAmount ) );

		//
		// Batch Node
		//
		{
			// generate sprite batch node
			auto sprite_batch_node = SpriteBatchNode::create( "textures/texture_001.png", VerticalAmount * HorizontalAmount );
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
				std::uniform_int_distribution<> dist( 0, SpriteFrames.size() - 1 );

				auto sprite_frame_indicator = 0u;
				for( int sy = 0; HorizontalAmount > sy; ++sy )
				{
					for( int sx = 0; VerticalAmount > sx; ++sx )
					{
						// update target sprite frame
						sprite_frame_indicator = dist( random_engine );

						// sprite
						temp_sprite->setSpriteFrame( SpriteFrames[sprite_frame_indicator] );
						temp_sprite->setPosition(
							( SpriteSize.width * 0.5f ) + ( SpriteSize.width * sx )
							, ( SpriteSize.height * 0.5f ) + ( SpriteSize.height * sy )
						);
						sprite_batch_node->insertQuadFromSprite( temp_sprite, sx + ( HorizontalAmount * sy ) );
					}
				}
			}
		}

		return true;
	}
}
