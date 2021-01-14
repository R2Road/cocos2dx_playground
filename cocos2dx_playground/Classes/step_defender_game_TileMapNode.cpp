#include "step_defender_game_TileMapNode.h"

#include <new>
#include <numeric>

#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"

USING_NS_CC;

namespace step_defender
{
	namespace game
	{
		TileMapNode::TileMapNode( const Config& config, const cpg::TileSheetConfiguration& tile_sheet_config ) :
			mConfig( config )
			, mTileSheetConfig( tile_sheet_config )
			, mTileSheetUtility()
			, mReusedSprite( nullptr )
		{}

		TileMapNode::~TileMapNode()
		{
			CC_SAFE_RELEASE( mReusedSprite );
		}

		TileMapNode* TileMapNode::create( const Config& config, const cpg::TileSheetConfiguration& tile_sheet_config )
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
			auto texture = _director->getTextureCache()->addImage( mTileSheetConfig.GetTexturePath() );
			CCASSERT( nullptr != texture, "Texture Nothing" );
			texture->setAliasTexParameters();

			if( !SpriteBatchNode::initWithTexture( texture, mConfig.MapWidth* mConfig.MapHeight ) )
			{
				return false;
			}

			const Size ContentSize( mConfig.MapWidth * mTileSheetConfig.GetTileWidth(), mConfig.MapHeight * mTileSheetConfig.GetTileHeight() );
			setContentSize( ContentSize );

			//
			// Setup - Texture & Tile Sheet Utility 
			//
			{
				mTileSheetUtility.Setup(
					mTileSheetConfig.GetTileWidth(), mTileSheetConfig.GetTileHeight()
					, mTileSheetConfig.GetTileMargin_Width(), mTileSheetConfig.GetTileMargin_Height()
					, texture->getContentSizeInPixels().height
				);
			}

			//
			// Reused Sprite
			//
			{
				mReusedSprite = Sprite::createWithTexture( texture );
				mReusedSprite->setAnchorPoint( Vec2::ZERO );
				mReusedSprite->setBatchNode( this );
				mReusedSprite->setScale( _director->getContentScaleFactor() );
				mReusedSprite->retain();
			}

			//
			// Setup Quads : TextureAtlas::insertQuad
			//
			{
				for( int ty = 0; mConfig.MapHeight > ty; ++ty )
				{
					for( int tx = 0; mConfig.MapWidth > tx; ++tx )
					{
						mReusedSprite->setTextureRect( Rect::ZERO );
						mReusedSprite->setPosition(
							mTileSheetConfig.GetTileWidth() * tx
							, mTileSheetConfig.GetTileHeight() * ty
						);
						insertQuadFromSprite( mReusedSprite, tx + ( mConfig.MapWidth * ty ) );
					}
				}
			}

			return true;
		}

		void TileMapNode::updateColor()
		{
			auto quads = _textureAtlas->getQuads();
			for( std::size_t cur = 0u, end = _textureAtlas->getTotalQuads(); end > cur; ++cur )
			{
				auto& q = quads[cur];

				q.tl.colors.a = _displayedOpacity;
				q.tl.colors.r = _displayedColor.r;
				q.tl.colors.g = _displayedColor.g;
				q.tl.colors.b = _displayedColor.b;

				q.tr.colors = q.tl.colors;
				q.bl.colors = q.tl.colors;
				q.br.colors = q.tl.colors;
			}
		}

		void TileMapNode::Reset()
		{
			removeAllChildrenWithCleanup( false );
		}

		void TileMapNode::FillAll( const int tile_point_x, const int tile_point_y )
		{
			const auto tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( tile_point_x, tile_point_y );
			for( int ty = 0; mConfig.MapHeight > ty; ++ty )
			{
				for( int tx = 0; mConfig.MapWidth > tx; ++tx )
				{
					mReusedSprite->setTextureRect( tile_rect );
					mReusedSprite->setPosition(
						mTileSheetConfig.GetTileWidth() * tx
						, mTileSheetConfig.GetTileHeight() * ty
					);
					updateQuadFromSprite( mReusedSprite, tx + ( mConfig.MapWidth * ty ) );
				}
			}
		}
		void TileMapNode::UpdateTile( const int map_point_x, const int map_point_y, const int tile_point_x, const int tile_point_y )
		{
			if( mConfig.MapWidth <= map_point_x || mConfig.MapHeight <= map_point_y )
			{
				return;
			}

			const auto tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( tile_point_x, tile_point_y );

			mReusedSprite->setTextureRect( tile_rect );
			mReusedSprite->setPosition(
				mTileSheetConfig.GetTileWidth() * map_point_x
				, mTileSheetConfig.GetTileHeight() * map_point_y
			);
			updateQuadFromSprite( mReusedSprite, map_point_x + ( mConfig.MapWidth * map_point_y ) );
		}
		void TileMapNode::EraseTile( const int map_point_x, const int map_point_y )
		{
			if( mConfig.MapWidth <= map_point_x || mConfig.MapHeight <= map_point_y )
			{
				return;
			}

			mReusedSprite->setTextureRect( Rect::ZERO );
			mReusedSprite->setPosition(
				mTileSheetConfig.GetTileWidth() * map_point_x
				, mTileSheetConfig.GetTileHeight() * map_point_y
			);
			updateQuadFromSprite( mReusedSprite, map_point_x + ( mConfig.MapWidth * map_point_y ) );
		}
	}
}
