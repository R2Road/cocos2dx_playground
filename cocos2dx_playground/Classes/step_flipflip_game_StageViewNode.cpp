#include "step_flipflip_game_StageViewNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"

#include "step_flipflip_game_CardViewNode.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		StageViewNode::StageViewNode() : mCardViewContainer()
		{}

		StageViewNode* StageViewNode::create( const int width, const int height )
		{
			auto ret = new ( std::nothrow ) StageViewNode();
			if( !ret || !ret->init( width, height ) )
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

		bool StageViewNode::init( const int width, const int height )
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size CardAreaSize( 40.f, 54.f );

			//
			// Content Size
			//
			setContentSize( Size( CardAreaSize.width * width, CardAreaSize.height * height ) );

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B::GREEN, getContentSize().width, getContentSize().height );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Build
			//
			{
				const Vec2 pivot_position( CardAreaSize.width * 0.5f, CardAreaSize.height * 0.5f );
				for( int current_h = 0; height > current_h; ++current_h )
				{
					for( int current_w = 0; width > current_w; ++current_w )
					{
						auto card_view_node = CardViewNode::create( eCardType::A );
						card_view_node->setPosition(
							pivot_position
							+ Vec2( CardAreaSize.width * current_w, CardAreaSize.height * current_h )
						);
						addChild( card_view_node );
					}
				}
			}

			return true;
		}
	}
}
