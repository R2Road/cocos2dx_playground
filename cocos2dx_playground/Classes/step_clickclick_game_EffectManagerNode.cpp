#include "step_clickclick_game_EffectManagerNode.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_clickclick_game_EffectView.h"
#include "cpg_GridIndexConverter.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		EffectManagerNode::EffectManagerNode()
		{
		}

		EffectManagerNode* EffectManagerNode::create( const Config config, const int width, const int height )
		{
			auto ret = new ( std::nothrow ) EffectManagerNode();
			if( !ret || !ret->init( config, width, height ) )
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

		bool EffectManagerNode::init( const Config config, const int width, const int height )
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size BlockSize( 32.f, 32.f );
			const Size MarginSize( 2.f, 2.f );
			const Size StageSize(
				( width * BlockSize.width ) + ( ( width - 1 ) * MarginSize.width )
				,( height * BlockSize.height ) + ( ( height - 1 ) * MarginSize.height )
			);
			const Vec2 pivot_position( StageSize.width * -0.5f, StageSize.height * -0.5f );

			setContentSize( StageSize );

			//
			// Background
			//
			if( config.bShowBackground )
			{
				auto pivot = LayerColor::create( Color4B::GRAY, getContentSize().width, getContentSize().height );
				pivot->setPosition( pivot_position );
				addChild( pivot, std::numeric_limits<int>::min() );
			}

			//
			// Build Effect View List
			//
			cpg::GridIndexConverter grid_index_converter( width, height );
			int linear_index = 0;
			EffectView* effect_view_node = nullptr;
			for( int ty = 0; ty < height; ++ty )
			{
				for( int tx = 0; tx < width; ++tx )
				{
					linear_index = grid_index_converter.To_Linear( tx, ty );

					effect_view_node = EffectView::create();
					effect_view_node->setTag( linear_index );
					effect_view_node->setPosition(
						pivot_position
						+ Vec2( BlockSize.width * 0.5f, BlockSize.height * 0.5f )
						+ Vec2( tx * ( BlockSize.width + MarginSize.width ), ty * ( BlockSize.height + MarginSize.height ) )
					);
					addChild( effect_view_node );
				}
			}

			return true;
		}


		void EffectManagerNode::PlayEffect( const int index, const EffectView::eAnimationIndex animation_index )
		{
			auto effect_view_node = static_cast<EffectView*>( getChildByTag( index ) );
			effect_view_node->PlayEffect( animation_index );
		}
	}
}
