#include "step_clickclick_game_EffectManagerNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "cpg_GridIndexConverter.h"
#include "step_clickclick_game_EffectViewNode.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		EffectManagerNode::EffectManagerNode()
		{}

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

			const Size StageSize(
				( width * BlockSize.width ) + ( ( width - 1 ) * BlockMarginSize.width )
				,( height * BlockSize.height ) + ( ( height - 1 ) * BlockMarginSize.height )
			);
			const Vec2 pivot_position( StageSize.width * -0.5f, StageSize.height * -0.5f );

			//
			// Background
			//
			if( config.bShowBackground )
			{
				auto layer = LayerColor::create( Color4B::GRAY, StageSize.width, StageSize.height );
				layer->setPosition( pivot_position );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Build Effect View List
			//
			cpg::GridIndexConverter grid_index_converter( width, height );
			int linear_index = 0;
			EffectViewNode* effect_view_node = nullptr;
			for( int ty = 0; ty < height; ++ty )
			{
				for( int tx = 0; tx < width; ++tx )
				{
					linear_index = grid_index_converter.To_Linear( tx, ty );

					effect_view_node = EffectViewNode::create();
					effect_view_node->setTag( linear_index );
					effect_view_node->setPosition(
						pivot_position
						+ Vec2( BlockSize.width * 0.5f, BlockSize.height * 0.5f )
						+ Vec2( tx * ( BlockSize.width + BlockMarginSize.width ), ty * ( BlockSize.height + BlockMarginSize.height ) )
					);
					addChild( effect_view_node );
				}
			}

			return true;
		}


		void EffectManagerNode::PlayEffect( const int index, const eEffectIndex effect_index )
		{
			auto effect_view_node = static_cast<EffectViewNode*>( getChildByTag( index ) );
			effect_view_node->PlayEffect( effect_index );
		}
	}
}
