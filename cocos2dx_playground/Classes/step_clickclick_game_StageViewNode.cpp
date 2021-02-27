#include "step_clickclick_game_StageViewNode.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_clickclick_game_BlockViewNode.h"
#include "step_clickclick_game_Stage.h"

USING_NS_CC;

namespace
{
#if defined( DEBUG ) || defined( _DEBUG )
	#define CHECK_ODD_NUMBER( number ) ( assert( 1 == ( ( number ) & 1 ) ) )
#else
	#define CHECK_ODD_NUMBER( number )
#endif
}

namespace step_clickclick
{
	namespace game
	{
		StageViewNode::StageViewNode( const int width, const int height ) :
			mStageWidth( width )
			, mStageHeight( height )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, mBlockViews()
		{}

		StageViewNode* StageViewNode::create( const Config config, const int width, const int height, const OnBlockCallback& on_block_callback )
		{
			//
			// Must odd number
			//
			CHECK_ODD_NUMBER( width );
			CHECK_ODD_NUMBER( height );

			auto ret = new ( std::nothrow ) StageViewNode( width, height );
			if( !ret || !ret->init( config, on_block_callback ) )
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

		bool StageViewNode::init( const Config config, const OnBlockCallback& on_block_callback )
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size StageSize(
				( mStageWidth * BlockSize.width ) + ( ( mStageWidth - 1 ) * BlockMarginSize.width )
				,( mStageHeight * BlockSize.height ) + ( ( mStageHeight - 1 ) * BlockMarginSize.height )
			);
			const Vec2 pivot_position( StageSize.width * -0.5f, StageSize.height * -0.5f );

			//
			// Pivot
			//
			if( config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			if( config.bShowBackgroundGuide )
			{
				auto pivot = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), StageSize.width, StageSize.height );
				pivot->setPosition( pivot_position );
				addChild( pivot, std::numeric_limits<int>::min() );
			}

			//
			// Build Block View Node List
			//
			{
				int linear_index = 0;
				BlockViewNode* block_view = nullptr;
				for( int ty = 0; ty < mStageHeight; ++ty )
				{
					for( int tx = 0; tx < mStageWidth; ++tx )
					{
						linear_index = mGridIndexConverter.To_Linear( tx, ty );

						block_view = BlockViewNode::create( linear_index, BlockSize, on_block_callback );
						block_view->setPosition(
							pivot_position
							+ Vec2( BlockSize.width * 0.5f, BlockSize.height * 0.5f )
							+ Vec2( tx * ( BlockSize.width + BlockMarginSize.width ), ty * ( BlockSize.height + BlockMarginSize.height ) )
						);
						addChild( block_view );

						mBlockViews.emplace_back( block_view );
					}
				}
			}

			return true;
		}

		void StageViewNode::Setup( const Stage& stage_data )
		{
			assert( mStageWidth == stage_data.GetWidth() );
			assert( mStageHeight == stage_data.GetHeight() );

			for( auto& b : mBlockViews )
			{
				b->setVisible( false );
			}

			for( const auto& b : stage_data.GetBlockDatas() )
			{
				if( !b.IsActive() )
				{
					continue;
				}

				mBlockViews[b.GetIndex()]->Reset( b.GetType(), b.GetLife() );
			}
		}

		void StageViewNode::UpdateBlock( const int linear_index,const int current_life )
		{
			mBlockViews[linear_index]->UpdateLife( current_life );
		}
	}
}
