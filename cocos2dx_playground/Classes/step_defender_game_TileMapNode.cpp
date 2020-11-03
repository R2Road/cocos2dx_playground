#include "step_defender_game_TileMapNode.h"

#include <new>
#include <numeric>

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

namespace step_defender
{
	namespace game
	{
		TileMapNode::TileMapNode( const Config& config ) : mConfig( config ), mSpriteBatchNode( nullptr ), mReusedSprite( nullptr )
		{}

		TileMapNode::~TileMapNode()
		{
			CC_SAFE_RELEASE( mReusedSprite );
		}

		TileMapNode* TileMapNode::create( const Config& config )
		{
			CCASSERT( 0 != config.MapWidth && 0 != config.MapHeight, "Failed - TileMapNode::create" );

			auto ret = new ( std::nothrow ) TileMapNode( config );
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

		bool TileMapNode::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size ContentSize( mConfig.MapWidth * mConfig.TileWidth, mConfig.MapHeight * mConfig.TileHeight );
			setContentSize( CC_SIZE_PIXELS_TO_POINTS( ContentSize ) );

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
				mSpriteBatchNode = SpriteBatchNode::create( mConfig.TexturePath, mConfig.MapWidth* mConfig.MapHeight );
				mSpriteBatchNode->setTag( TAG_BatchNode );
				addChild( mSpriteBatchNode );
			}

			//
			// Reused Sprite
			//
			{
				mReusedSprite = Sprite::createWithTexture( mSpriteBatchNode->getTexture()  );
				mReusedSprite->setBatchNode( mSpriteBatchNode );
				mReusedSprite->retain();
			}

			auto sprite_frame_indicator = 0u;
			for( int sy = 0; mConfig.MapHeight > sy; ++sy )
			{
				for( int sx = 0; mConfig.MapWidth > sx; ++sx )
				{
					mReusedSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_rain_of_chaos_tile_01_1.png" ) );
					mReusedSprite->setScale( _director->getContentScaleFactor() );
					mReusedSprite->setPosition(
						( mConfig.TileWidth * 0.5f ) + ( mConfig.TileWidth * sx )
						, ( mConfig.TileHeight * 0.5f ) + ( mConfig.TileHeight * sy )
					);
					mSpriteBatchNode->insertQuadFromSprite( mReusedSprite, sx + ( mConfig.MapWidth * sy ) );
				}
			}

			return true;
		}

		void TileMapNode::Reset()
		{
			auto sprite_batch_node = static_cast<SpriteBatchNode*>( getChildByTag( TAG_BatchNode ) );
			sprite_batch_node->removeAllChildrenWithCleanup( false );
		}
	}
}