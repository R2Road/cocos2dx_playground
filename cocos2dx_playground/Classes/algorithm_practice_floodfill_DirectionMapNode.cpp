#include "algorithm_practice_floodfill_DirectionMapNode.h"

#include <new>
#include <numeric>

#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCTextureCache.h"

USING_NS_CC;

namespace algorithm_practice_floodfill
{
	DirectionMapNode::DirectionMapNode( const Config& config ) :
		mConfig( config )
		, mTileSheetConfig()
		, mTileSheetUtility()
		, mReusedSprite( nullptr )
	{}

	DirectionMapNode::~DirectionMapNode()
	{
		CC_SAFE_RELEASE( mReusedSprite );
	}

	DirectionMapNode* DirectionMapNode::create( const Config& config )
	{
		CCASSERT( 0 != config.MapWidth && 0 != config.MapHeight, "Failed - DirectionMapNode::create" );

		auto ret = new ( std::nothrow ) DirectionMapNode( config );
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

	bool DirectionMapNode::init()
	{
		//
		// Load Tile Config
		//
		CCASSERT( mTileSheetConfig.Load( "datas/algorithm_practice/algorithm_practice_tile_sheet_config_01.json" ), "Failed - Load Tile Sheet Configuration" );

		auto texture = _director->getTextureCache()->addImage( mTileSheetConfig.GetTexturePath() );
		CCASSERT( nullptr != texture, "Texture Nothing" );
		texture->setAliasTexParameters();

		if( !SpriteBatchNode::initWithTexture( texture, ( mConfig.MapWidth* mConfig.MapHeight * 5 ) + 5 ) )
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
			int quad_index = -1;
			const auto tile_rect = Rect::ZERO;
			for( int ty = 0; mConfig.MapHeight > ty; ++ty )
			{
				for( int tx = 0; mConfig.MapWidth > tx; ++tx )
				{
					for( int dir = 0; 5 > dir; ++dir )
					{
						mReusedSprite->setTextureRect( tile_rect );
						mReusedSprite->setPosition(
							mTileSheetConfig.GetTileWidth() * tx
							, mTileSheetConfig.GetTileHeight() * ty
						);

						quad_index = ConvertPoint2QuadIndex( tx, ty, dir );
						insertQuadFromSprite( mReusedSprite, quad_index );
					}
				}
			}
		}

		return true;
	}
	void DirectionMapNode::updateColor()
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


	std::size_t DirectionMapNode::ConvertPoint2QuadIndex( const int point_x, const int point_y, const int offset ) const
	{
		return ( ( ( point_x * 5 ) + offset ) ) + ( mConfig.MapWidth * 5 * point_y );
	}
	Rect DirectionMapNode::ConvertDirection2TileRect( const cpg::Direction4::eState direction_type ) const
	{
		cocos2d::Rect tile_rect;

		switch( direction_type )
		{
		case cpg::Direction4::eState::Up:
			tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( 1, 3 );
			break;
		case cpg::Direction4::eState::Right:
			tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( 2, 3 );
			break;
		case cpg::Direction4::eState::Down:
			tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( 3, 3 );
			break;
		case cpg::Direction4::eState::Left:
			tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( 4, 3 );
			break;
		default:
			tile_rect = Rect::ZERO;
			break;
		}

		return tile_rect;
	}


	void DirectionMapNode::Reset()
	{
		int quad_index = -1;
		const auto tile_rect = Rect::ZERO;
		for( int ty = 0; mConfig.MapHeight > ty; ++ty )
		{
			for( int tx = 0; mConfig.MapWidth > tx; ++tx )
			{
				for( int dir = 0; 5 > dir; ++dir )
				{
					mReusedSprite->setTextureRect( tile_rect );
					mReusedSprite->setPosition(
						mTileSheetConfig.GetTileWidth() * tx
						, mTileSheetConfig.GetTileHeight() * ty
					);

					quad_index = ConvertPoint2QuadIndex( tx, ty, dir );
					updateQuadFromSprite( mReusedSprite, quad_index );
				}
			}
		}
	}

	void DirectionMapNode::UpdateTile( const int point_x, const int point_y, const char direction )
	{
		if( mConfig.MapWidth <= point_x || mConfig.MapHeight <= point_y )
		{
			return;
		}

		Rect tile_rect;

		//
		// Pivot
		//
		{
			tile_rect = mTileSheetUtility.ConvertTilePoint2TextureRect( 0, 3 );

			mReusedSprite->setTextureRect( tile_rect );
			mReusedSprite->setPosition(
				mTileSheetConfig.GetTileWidth() * point_x
				, mTileSheetConfig.GetTileHeight() * point_y
			);
			updateQuadFromSprite( mReusedSprite, ConvertPoint2QuadIndex( point_x, point_y, 4 ) );
		}

		//
		// 4 - Direction
		//
		int result_direction = -1;
		for( int dir = 0; 4 > dir; ++dir )
		{
			result_direction = direction & ( 1 << dir );

			tile_rect = ConvertDirection2TileRect( static_cast<cpg::Direction4::eState>( result_direction ) );
			mReusedSprite->setTextureRect( tile_rect );
			mReusedSprite->setPosition(
				mTileSheetConfig.GetTileWidth() * point_x
				, mTileSheetConfig.GetTileHeight() * point_y
			);
			updateQuadFromSprite( mReusedSprite, ConvertPoint2QuadIndex( point_x, point_y, dir ) );
		}
	}
	void DirectionMapNode::ClearTile( const int point_x, const int point_y )
	{
		if( mConfig.MapWidth <= point_x || mConfig.MapHeight <= point_y )
		{
			return;
		}

		int quad_index = -1;
		const auto tile_rect = Rect::ZERO;
		for( int dir = 0; 5 > dir; ++dir )
		{
			mReusedSprite->setTextureRect( tile_rect );
			mReusedSprite->setPosition(
				mTileSheetConfig.GetTileWidth() * point_x
				, mTileSheetConfig.GetTileHeight() * point_y
			);

			quad_index = ConvertPoint2QuadIndex( point_x, point_y, dir );
			updateQuadFromSprite( mReusedSprite, quad_index );
		}
	}
}
