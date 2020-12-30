#include "step_clickclick_game_StageView.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_clickclick_game_BlockViewNode.h"
#include "step_clickclick_game_Stage.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		namespace
		{
#if defined( DEBUG ) || defined( _DEBUG )
		#define CHECK_ODD_NUMBER( number ) ( assert( 1 == ( ( number ) & 1 ) ) )
#else
		#define CHECK_ODD_NUMBER( number )
#endif
		}

		StageView::StageView( const int width, const int height ) :
			mStageWidth( width )
			, mStageHeight( height )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, mBlockViews()
		{
			//
			// Must odd number
			//
			CHECK_ODD_NUMBER( mStageWidth );
			CHECK_ODD_NUMBER( mStageHeight );
		}

		StageView* StageView::create( const int width, const int height, const OnBlockCallback& on_block_callback, const StageViewConfig config )
		{
			auto ret = new ( std::nothrow ) StageView( width, height );
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

		bool StageView::init( const StageViewConfig config, const OnBlockCallback& on_block_callback )
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

			setContentSize( StageSize );

			if( config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			if( config.bShowBackgroundGuide )
			{
				auto pivot = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
				pivot->setPosition( pivot_position );
				addChild( pivot, std::numeric_limits<int>::min() );
			}

			// Buttons
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

			return true;
		}

		void StageView::Setup( const Stage& stage_data )
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

		void StageView::UpdateBlock( const int linear_index, const int last_life, const int current_life )
		{
			mBlockViews[linear_index]->UpdateLife( last_life, current_life );
		}
	} // namespace game
} // namespace step_clickclick
