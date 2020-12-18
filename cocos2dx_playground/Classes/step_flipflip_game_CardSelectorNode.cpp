#include "step_flipflip_game_CardSelectorNode.h"

#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"

#include "step_flipflip_game_Constant.h"

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		CardSelectorNode::CardSelectorNode( const int width, const int height ) :
			mWidth( width )
			, mHeight( height )
			, mPivotPosition( step_flipflip::game::STAGE_CONFIG.CardAreaSize.width * 0.5f, step_flipflip::game::STAGE_CONFIG.CardAreaSize.height * 0.5f )

			, mCurrentX( 0 )
			, mCurrentY( 0 )
			, mIndicator( nullptr )
		{}

		CardSelectorNode* CardSelectorNode::create( const int width, const int height, const bool show_guide )
		{
			auto ret = new ( std::nothrow ) CardSelectorNode( width, height );
			if( !ret || !ret->init( show_guide ) )
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

		bool CardSelectorNode::init( const bool show_guide )
		{
			if( !Node::init() )
			{
				return false;
			}

			//
			// Content Size
			//
			setContentSize( Size( step_flipflip::game::STAGE_CONFIG.CardAreaSize.width * mWidth, step_flipflip::game::STAGE_CONFIG.CardAreaSize.height * mHeight ) );

			//
			// Debug Guide
			//
			if( show_guide )
			{
				auto layer = LayerColor::create( Color4B::GRAY, getContentSize().width, getContentSize().height );
				addChild( layer, std::numeric_limits<int>::min() );
			}

			//
			// Build
			//
			{
				mIndicator = Sprite::createWithSpriteFrameName( step_flipflip::game::GetSpriteFrameName_CardIndicator() );
				addChild( mIndicator );
			}

			//
			// Setup
			//
			MoveIndicator( 0, 0 );

			return true;
		}

		void CardSelectorNode::MoveIndicator( const int move_amount_x, const int move_amount_y )
		{
			mCurrentX = std::max(
				std::min( mCurrentX + move_amount_x, mWidth - 1 )
				, 0
			);
			mCurrentY = std::max(
				std::min( mCurrentY + move_amount_y, mHeight - 1 )
				, 0
			);

			mIndicator->setPosition(
				mPivotPosition
				+ Vec2( step_flipflip::game::STAGE_CONFIG.CardAreaSize.width * mCurrentX, step_flipflip::game::STAGE_CONFIG.CardAreaSize.height * mCurrentY )
			);
		}
	}
}
