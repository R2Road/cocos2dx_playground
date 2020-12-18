#include "step_flipflip_game_StageViewNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"

#include "step_flipflip_game_CardViewNode.h"
#include "step_flipflip_game_Constant.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		StageViewNode::StageViewNode() : mCardViewContainer()
		{}

		StageViewNode* StageViewNode::create( const int width, const int height, const bool show_guide )
		{
			auto ret = new ( std::nothrow ) StageViewNode();
			if( !ret || !ret->init( width, height, show_guide ) )
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

		bool StageViewNode::init( const int width, const int height, const bool show_guide )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Content Size
			//
			setContentSize( Size( step_flipflip::game::STAGE_CONFIG.CardAreaSize.width * width, step_flipflip::game::STAGE_CONFIG.CardAreaSize.height * height ) );

			//
			// Background
			//
			if( show_guide )
			{
				auto layer = LayerColor::create( Color4B::GREEN, getContentSize().width, getContentSize().height );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Build
			//
			{
				const Vec2 pivot_position( step_flipflip::game::STAGE_CONFIG.CardAreaSize.width * 0.5f, step_flipflip::game::STAGE_CONFIG.CardAreaSize.height * 0.5f );
				for( int current_h = 0; height > current_h; ++current_h )
				{
					for( int current_w = 0; width > current_w; ++current_w )
					{
						auto card_view_node = CardViewNode::create( eCardType::A );
						card_view_node->setPosition(
							pivot_position
							+ Vec2( step_flipflip::game::STAGE_CONFIG.CardAreaSize.width * current_w, step_flipflip::game::STAGE_CONFIG.CardAreaSize.height * current_h )
						);
						addChild( card_view_node );
					}
				}
			}

			return true;
		}
	}
}
