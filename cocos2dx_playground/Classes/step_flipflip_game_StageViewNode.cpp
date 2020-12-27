#include "step_flipflip_game_StageViewNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"

#include "step_flipflip_game_CardViewNode.h"
#include "step_flipflip_game_Constant.h"
#include "step_flipflip_game_StageData.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		StageViewNode::StageViewNode() :
			mIndexConverter( 1, 1 )
			, mCardViewContainer()
			, mLastFlipedX( -1 )
			, mLastFlipedY( -1 )
		{}

		StageViewNode* StageViewNode::create( const StageConfig& stage_config, const StageData& stage_data, const bool show_guide )
		{
			auto ret = new ( std::nothrow ) StageViewNode();
			if( !ret || !ret->init( stage_config, stage_data, show_guide ) )
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

		bool StageViewNode::init( const StageConfig& stage_config, const StageData& stage_data, const bool show_guide )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Content Size
			//
			setContentSize( Size( stage_config.CardAreaSize.width * stage_config.Width, stage_config.CardAreaSize.height * stage_config.Height ) );

			//
			// Background
			//
			if( show_guide )
			{
				auto layer = LayerColor::create( Color4B::BLACK, getContentSize().width, getContentSize().height );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Index Converter
			//
			mIndexConverter = cpg::GridIndexConverter( stage_config.Width, stage_config.Height);

			//
			// Build
			//
			{
				const Vec2 pivot_position( stage_config.CardAreaSize.width * 0.5f, stage_config.CardAreaSize.height * 0.5f );
				for( int current_h = 0; stage_config.Height > current_h; ++current_h )
				{
					for( int current_w = 0; stage_config.Width > current_w; ++current_w )
					{
						auto card_view_node = CardViewNode::create( stage_data.GetType( current_w, current_h ) );
						card_view_node->setPosition(
							pivot_position
							+ Vec2( stage_config.CardAreaSize.width * current_w, stage_config.CardAreaSize.height * current_h )
						);
						addChild( card_view_node );

						mCardViewContainer.push_back( card_view_node );
					}
				}
			}

			return true;
		}

		void StageViewNode::HideAll()
		{
			for( auto& c : mCardViewContainer )
			{
				if( c->IsOpen() )
				{
					c->Flip();
				}
			}
		}
		void StageViewNode::Flip( const int x, const int y )
		{
			mLastFlipedX = x;
			mLastFlipedY = y;

			mCardViewContainer[mIndexConverter.To_Linear( mLastFlipedX, mLastFlipedY )]->Flip();
		}
		bool StageViewNode::isFlipping()
		{
			return mCardViewContainer[mIndexConverter.To_Linear( mLastFlipedX, mLastFlipedY )]->IsFlipping();
		}
	}
}
