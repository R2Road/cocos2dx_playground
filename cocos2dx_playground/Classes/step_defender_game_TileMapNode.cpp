#include "step_defender_game_TileMapNode.h"

#include <new>
#include <numeric>

#include "2d/CCSprite.h"
#include "2d/CCSpriteBatchNode.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"

USING_NS_CC;

namespace
{
	const int TAG_BatchNode = 20140416;
}

namespace step_defender
{
	namespace game
	{
		TileMapNode::TileMapNode( const Config& config, const TileSheetConfiguration& tile_sheet_config ) :
			mConfig( config )
			, mTileSheetConfig( tile_sheet_config )
			, mTileSheetUtility()
			, mSpriteBatchNode( nullptr )
			, mReusedSprite( nullptr )
		{}

		TileMapNode::~TileMapNode()
		{
			CC_SAFE_RELEASE( mReusedSprite );
		}

		TileMapNode* TileMapNode::create( const Config& config, const TileSheetConfiguration& tile_sheet_config )
		{
			CCASSERT( 0 != config.MapWidth && 0 != config.MapHeight, "Failed - TileMapNode::create" );

			auto ret = new ( std::nothrow ) TileMapNode( config, tile_sheet_config );
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

			const Size ContentSize( mConfig.MapWidth * mTileSheetConfig.TileWidth, mConfig.MapHeight * mTileSheetConfig.TileHeight );
			setContentSize( ContentSize );

			//
			// Pivot
			//
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Tile Sheet Utility
			//
			{
				auto texture = _director->getTextureCache()->addImage( game::TileSheetConfig.TexturePath );
				CCASSERT( nullptr != texture, "Texture Nothing" );

				mTileSheetUtility.Setup( game::TileSheetConfig.TileWidth, game::TileSheetConfig.TileHeight, game::TileSheetConfig.TileMargin_Width, game::TileSheetConfig.TileMargin_Height, texture->getContentSizeInPixels().height );
			}

			//
			// Generate Sprite Batch Node
			//
			{
				mSpriteBatchNode = SpriteBatchNode::create( mTileSheetConfig.TexturePath, mConfig.MapWidth* mConfig.MapHeight );
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

			//
			// Setup Quads : TextureAtlas::insertQuad
			//
			{
				const auto tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( 0, 0 );
				for( int sy = 0; mConfig.MapHeight > sy; ++sy )
				{
					for( int sx = 0; mConfig.MapWidth > sx; ++sx )
					{
						mReusedSprite->setTextureRect( tile_rect );
						mReusedSprite->setScale( _director->getContentScaleFactor() );
						mReusedSprite->setPosition(
							( TileSheetConfig.TileWidth * 0.5f ) + ( TileSheetConfig.TileWidth * sx )
							, ( TileSheetConfig.TileHeight * 0.5f ) + ( TileSheetConfig.TileHeight * sy )
						);
						mSpriteBatchNode->insertQuadFromSprite( mReusedSprite, sx + ( mConfig.MapWidth * sy ) );
					}
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
