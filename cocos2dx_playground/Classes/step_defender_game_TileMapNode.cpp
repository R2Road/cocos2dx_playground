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
				for( int ty = 0; mConfig.MapHeight > ty; ++ty )
				{
					for( int tx = 0; mConfig.MapWidth > tx; ++tx )
					{
						mReusedSprite->setTextureRect( tile_rect );
						mReusedSprite->setScale( _director->getContentScaleFactor() );
						mReusedSprite->setPosition(
							( TileSheetConfig.TileWidth * 0.5f ) + ( TileSheetConfig.TileWidth * tx )
							, ( TileSheetConfig.TileHeight * 0.5f ) + ( TileSheetConfig.TileHeight * ty )
						);
						mSpriteBatchNode->insertQuadFromSprite( mReusedSprite, tx + ( mConfig.MapWidth * ty ) );
					}
				}
			}

			return true;
		}

		void TileMapNode::Reset()
		{
			mSpriteBatchNode->removeAllChildrenWithCleanup( false );
		}

		void TileMapNode::FillAll( const int tile_point_x, const int tile_point_y )
		{
			Reset();

			const auto tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( tile_point_x, tile_point_y );
			for( int ty = 0; mConfig.MapHeight > ty; ++ty )
			{
				for( int tx = 0; mConfig.MapWidth > tx; ++tx )
				{
					mReusedSprite->setTextureRect( tile_rect );
					mReusedSprite->setScale( _director->getContentScaleFactor() );
					mReusedSprite->setPosition(
						( TileSheetConfig.TileWidth * 0.5f ) + ( TileSheetConfig.TileWidth * tx )
						, ( TileSheetConfig.TileHeight * 0.5f ) + ( TileSheetConfig.TileHeight * ty )
					);
					mSpriteBatchNode->insertQuadFromSprite( mReusedSprite, tx + ( mConfig.MapWidth * ty ) );
				}
			}
		}
	}
}
