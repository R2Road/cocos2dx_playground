#include "step_clickclick_game_StageView.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/ccMacros.h"
#include "ui/UIButton.h"
#include "2d/CCSpriteFrameCache.h"

#include "step_clickclick_game_BlockView.h"
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

		StageView::StageView( const int width, const int height, const OnBlockCallback& on_block_callback ) :
			mStageWidth( width )
			, mStageHeight( height )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, mBlockViews()
			, mOnBlockCallback( on_block_callback )
		{
			//
			// Must odd number
			//
			CHECK_ODD_NUMBER( mStageWidth );
			CHECK_ODD_NUMBER( mStageHeight );
		}

		StageView* StageView::create( const int width, const int height, const OnBlockCallback& on_block_callback, const StageViewConfig config )
		{
			auto ret = new ( std::nothrow ) StageView( width, height, on_block_callback );
			if( !ret || !ret->init( config ) )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool StageView::init( const StageViewConfig config )
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size tile_size( 32.f, 32.f );
			const Size margin_size( 2.f, 2.f );
			const Size stage_size(
				( mStageWidth * tile_size.width ) + ( ( mStageWidth - 1 ) * margin_size.width )
				,( mStageHeight * tile_size.height ) + ( ( mStageHeight - 1 ) * margin_size.height )
			);
			const Vec2 pivot_position( stage_size.width * -0.5f, stage_size.height * -0.5f );

			setContentSize( stage_size );

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
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					linear_index = mGridIndexConverter.To_Linear( tx, ty );

					auto block_view = BlockView::create( linear_index, tile_size, mOnBlockCallback );
					block_view->setPosition(
						pivot_position
						+ Vec2( tile_size.width * 0.5f, tile_size.height * 0.5f )
						+ Vec2( tx * ( tile_size.width + margin_size.width ), ty * ( tile_size.height + margin_size.height ) )
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
				b->SetVisible( false );
			}

			for( const auto& b : stage_data.GetBlockDatas() )
			{
				if( !b.IsActive() )
				{
					continue;
				}

				mBlockViews[b.GetIndex()]->Reset( b.GetType(), b.GetLife() );
				mBlockViews[b.GetIndex()]->SetVisible( true );
			}
		}

		void StageView::UpdateBlock( const int linear_index, const int last_life, const int current_life )
		{
			mBlockViews[linear_index]->UpdateLife( last_life, current_life );
		}

		void StageView::onBlock( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto button_node = static_cast<Node*>( sender );
			mOnBlockCallback( button_node->getTag() );
		}
	} // namespace game
} // namespace step_clickclick
